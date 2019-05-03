/* DiskTableBackend.h
  
   A simple implementation of a stored table with a fixed number of columns
   and an unbounded number of rows, where the size of each data entry
   in each row is also unbounded. Each element is stored as a string (although
   there is some infrastructure to support future typed storage.
   
   This version uses 32 bit sizes, giving the following limits:
    - Maximum rows: 2^32 - 1
    - Maximum number of bytes of data in a row: 2^32-1 - m (where m is the number of columns)
    
   The file starts with a 32-bit magic number 0xC5C490DB, followed by a header in
   the following format:
    - A 32-bit big-endian row count (n)
    - A 32-bit big-endian column count (m)
    - A sequence of m pairs (column_name, column_type) of null terminated strings 
      describing each of the columns of the table (in order).
   The header is followed by a record for each of the n rows. Each row record
   contains the following data
    - The 32-bit big-endian length (in bytes) of the row data chunk
    - The row data chunk: A sequence of m null terminated strings.
    - The 32-bit big-endian length (in bytes) of the row data chunk (again)
   Note that the size of the row data chunk is stored twice: once at the beginning and 
   once at the end of the data itself. The size value only counts the bytes that are
   part of the data chunk itself (that is, it does not include the bytes used by the 
   two 32-bit size values).
   
   So, a typical file will have a layout like the following (where n is the number of rows,
   m is the number of columns, and the names k1, k2, ..., kn are the lengths, in bytes, of
   the data chunks of rows 1, 2, ..., n, respectively)
   
   -------------------------------------------------------------------------------------
   | magic | n | m | col 1 name | \0 | col 1 type | \0 | ... | col m name | col m type | 
   -------------------------------------------------------------------------------------
   | row 1 size (k1) |       row 1 data (k1 bytes in length)         | row 1 size (k1) |   
   -------------------------------------------------------------------------------------
   | row 2 size (k2) |       row 2 data (k2 bytes in length)         | row 2 size (k2) |   
   -------------------------------------------------------------------------------------
   | row 3 size (k3) |       row 3 data (k3 bytes in length)         | row 3 size (k3) |  
   ------------------------------------------------------------------------------------- 
   |                                       ...                                         |  
   -------------------------------------------------------------------------------------  
   | row n size (kn) |       row n data (kn bytes in length)         | row n size (kn) |  
   -------------------------------------------------------------------------------------     
   
   n.b. The reason that the row size is stored twice is to facilitate both forwards and 
   reverse iteration through the file (which is not currently needed, but would be needed
   in the future for features like deleting rows).
   
   B. Bird - 12/17/2018
*/ 


#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <iomanip>
#include <memory>

#include <cstdint> //For uint32_t


class DiskTableInvalidException: public std::runtime_error {
public:
    DiskTableInvalidException( const std::string& message ): std::runtime_error {message} {
    }
};

class DiskTableBackend;
class RowReader;
using DiskTablePtr = std::shared_ptr<DiskTableBackend>;

//Inheriting from enable_shared_from_this adds a method shared_from_this 
//which returns a shared pointer to the current object.
//See https://en.cppreference.com/w/cpp/memory/enable_shared_from_this
class DiskTableBackend: public std::enable_shared_from_this<DiskTableBackend>{
public:
    using recordsize_t = uint32_t;
    
    /* To create a DiskTableBackend object, call one of the static functions below. */
    /* See test1.cpp for a use case of create and diag_insert_rows.cpp for a use case of load */
    static DiskTablePtr create( std::string filename, 
                                std::vector<std::string> column_names, 
                                std::vector<std::string> column_types ) {
        //See https://stackoverflow.com/questions/8147027/how-do-i-call-stdmake-shared-on-a-class-with-only-protected-or-private-const/8147213#8147213
        class DiskTableBackendProxy: public DiskTableBackend {};
        DiskTablePtr dt = std::make_shared<DiskTableBackendProxy>();
        dt->column_names = column_names;
        dt->column_types = column_types;
        dt->initialize_file(filename);
        return dt;
    }
    static DiskTablePtr load( std::string filename ){
        class DiskTableBackendProxy: public DiskTableBackend {};
        DiskTablePtr dt = std::make_shared<DiskTableBackendProxy>();
        dt->open_and_read_header(filename);
        return dt;
    }
    
    
    const std::vector<std::string>& get_column_names(){
        return column_names;
    }
    const std::vector<std::string>& get_column_types(){
        return column_types;
    }
    unsigned int get_num_columns(){
        return column_names.size();
    }
    
    recordsize_t get_num_rows(){
        //Note that there are two different internal positions maintained
        //by the file object: The "output position" (get with tellp(), set with seekp())
        //and the "input position" (get with tellg(), set with seekg()).
        //The output position is used by all functions that write and the input position
        //is used by all functions that read.
        
        file.seekg( sizeof(recordsize_t) );
        try{
            return read_rsize();
        }catch(DiskTableInvalidException& e){
            throw DiskTableInvalidException("Unable to read number of rows");
        }
    }
    
    //Append a new row to the end of the file
    void write_row( std::vector<std::string> row ){
        if (row.size() != get_column_names().size())
            throw DiskTableInvalidException("Wrong number of columns in write_row");
        auto old_row_count = get_num_rows();
        file.seekp(0, std::ios_base::end);
        recordsize_t size = 0;
        for (auto& r: row){
            size += r.length() + 1; //+1 to account for null terminator
        }
        write_rsize(size);
        for(auto& r: row){
            write_null_terminated(r);
        }
        write_rsize(size);
        //Now update the row count
        file.seekp( row_count_offset );
        write_rsize( old_row_count + 1 );
    }
    
    

    
    class RowReader{
    public:

	// Placeholder constructor with zero arguments
	RowReader(){}

        //These are duplicates of the methods in DiskTableBackend, provided for convenience
        const std::vector<std::string>& get_column_names(){
            return parent_table->column_names;
        }
        
        const std::vector<std::string>& get_column_types(){
            return parent_table->column_types;
        }        
        
        recordsize_t get_num_rows(){
            return parent_table->get_num_rows();
        }
        
        recordsize_t get_num_columns(){
            return parent_table->get_num_columns();
        }
        
        bool has_next_row(){
            return parent_table->offset_valid(offset);
        }
        
        std::vector<std::string> get_next_row(){
            if (!has_next_row())
                throw DiskTableInvalidException("No more rows");
            return parent_table->read_row(offset);
        }

    private:
        RowReader(DiskTablePtr parent_table, std::fstream::pos_type start_offset): parent_table{parent_table}, offset{start_offset}{
        }


        DiskTablePtr parent_table;
        std::fstream::pos_type offset;
        
        friend class DiskTableBackend;
    };

    
    RowReader get_reader(){
       RowReader r { shared_from_this(), first_record_position };
        return r;
    }

    
protected:
    //The constructor is protected to prevent anyone from creating this class
    //except through the create() and load() static member functions above.
    DiskTableBackend(){
    }
private:
    using recordsize_bytes = std::array<char, sizeof(recordsize_t)>;
    
    static const recordsize_t magic_number = 0xC5C490DB;
    
    static const unsigned int row_count_offset = sizeof(recordsize_t);
    static const unsigned int col_count_offset = row_count_offset + sizeof(recordsize_t);
    

    
    //Open the provide file and 
    //read the header fields (magic number, rows, columns, column names/types) from an existing file
    void open_and_read_header(std::string filename){
        file.open(filename, file.binary | file.in | file.out );
        if (!file.is_open())
            throw DiskTableInvalidException("Unable to open file " + filename);
        file.seekg(0);
        auto magic = read_rsize();
        if (magic != magic_number)
            throw DiskTableInvalidException("Magic number invalid");
        
        /* auto num_rows = read_rsize(); */
        read_rsize(); //Read and discard the number of rows
        
        auto num_cols = read_rsize();
        
        //TODO maybe this isn't an error
        if (num_cols == 0)
            throw DiskTableInvalidException("Table with zero columns");
        
        column_names.clear();
        column_types.clear();
        for (unsigned int i = 0; i < num_cols; i++){
            column_names.push_back( read_null_terminated() );
            column_types.push_back( read_null_terminated() );
        }
        first_record_position = file.tellg();
        
    }
    
    //Open the provided file (deleting its contents if it already exists) and
    //write the header fields to the file (assuming that the table is initially empty)
    void initialize_file(std::string filename){
        file.open(filename, file.binary | file.in | file.out | file.trunc );
        if (!file.is_open())
            throw DiskTableInvalidException("Unable to open file " + filename);
        file.seekp(0);
        write_rsize(magic_number);
        write_rsize(0); //Row count
        write_rsize( get_num_columns() ); //Column count
        for (unsigned int i = 0; i < get_num_columns(); i++){
            write_null_terminated( column_names.at(i) );
            write_null_terminated( column_types.at(i) );
        }
        file.seekg(file.tellp());
        first_record_position = file.tellg();        
    }
    
    std::string read_null_terminated(){
        std::string result {};
        char c {};
        auto start_offset = file.tellg();
        while(1){
            if (!file.get(c))
                throw DiskTableInvalidException("Unable to read null terminated string starting at offset "+ std::to_string(start_offset) + ": Error at index " + std::to_string(result.length()));
            if (c == '\0')
                break;
            result.push_back(c);
        }
        return result;
    }
    
    void write_null_terminated(std::string s){
        auto start_offset = file.tellp();
        for(unsigned int i = 0; i <= s.length(); i++){ //Note the <=
            char c = '\0';
            if (i < s.length())
                c = s.at(i);
            if (!file.put(c))
                throw DiskTableInvalidException("Unable to write null terminated string starting at offset "+ std::to_string(start_offset) + ": Error at index " + std::to_string(i));
        }
                
    }
    
    recordsize_t read_rsize(){
        recordsize_bytes bytes;
        auto start_offset = file.tellg();
        for(unsigned int i = 0; i < bytes.size(); i++){
            if (!file.get(bytes[i])){
                throw DiskTableInvalidException("Unable to read record starting at offset "+ std::to_string(start_offset) );
            }
        }
        return decode_record_size(bytes);
    };
    
    void write_rsize(recordsize_t r){
        recordsize_bytes bytes = encode_record_size(r);
        for(auto b: bytes){
            if (!file.put(b)){
                throw DiskTableInvalidException("Unable to write byte");
            }
        }
    }
    
    //Read a row from the provided start position (assumed to be the offset of the row size, which is followed
    //by the row itself, then another copy of the row size), 
    //then update the offset to be the position of the next row.
    std::vector<std::string> read_row(std::fstream::pos_type& offset){
        file.seekg(offset);
        //auto record_length = read_rsize();
        read_rsize(); //Read and discard record length
        
        std::vector<std::string> row {};
        for(unsigned int col = 0; col < get_num_columns(); col++){
            try{
                row.push_back( read_null_terminated() );
            }catch(DiskTableInvalidException& e ){
                throw DiskTableInvalidException( "Error while reading column " + std::to_string(col) + ": " + e.what() );
            }
        }
        read_rsize(); //Read and discard record length
        offset = file.tellg();
        return row;
    }
    
    //Returns true if the offset refers to a valid position in the file.
    bool offset_valid(std::fstream::pos_type offset){
        //It's probably risky to try and test for eof status, so instead we can just find the location
        //of the end and compare it with the offset provided.
        file.seekg(0, std::ios_base::end);
        auto end_offset = file.tellg();
        return end_offset > offset;
    }
    
    
    

    static inline recordsize_bytes encode_record_size( recordsize_t s ){
        recordsize_bytes result;
        for (int i = (int)result.size() - 1; i >= 0; i--){
            result[i] = s&0xff;
            s = s >> 8;
        }
        return result;
    }
    static inline recordsize_t decode_record_size( recordsize_bytes bytes ){
        recordsize_t result;
        for( auto b: bytes ){
            result = result << 8;
            result |= (unsigned char)b;
        }
        return result;
    }
    
    std::fstream::pos_type first_record_position;
    
    std::vector<std::string> column_names;
    std::vector<std::string> column_types;
    std::fstream file;
};
