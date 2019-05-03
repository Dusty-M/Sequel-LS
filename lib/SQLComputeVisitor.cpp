#include "SQLComputeVisitor.h"

/********************************************************
	The methods below all return a vector of BaseTblPtr.
********************************************************/
antlrcpp::Any SQLComputeVisitor::visitPrimary_expression(SQLParser::Primary_expressionContext *ctx) {
	// Visit children.
	ctx->statement()->accept(this);

	// Get the results of select statements.
	std::vector<BaseTblPtr> results = tbl_dir.get_select_results();
	return results;
}

/*************************************************
	The method below returns a placeholder 0.
*************************************************/
antlrcpp::Any SQLComputeVisitor::visitStatementSingle(SQLParser::StatementSingleContext *ctx) {
	std::pair<TableDirectory::table_type, BaseTblPtr> first_statement = ctx->first_statement->accept(this);
	tbl_dir.register_tbl(first_statement);
	return 0;
}

antlrcpp::Any SQLComputeVisitor::visitStatementMultiple(SQLParser::StatementMultipleContext *ctx) {
	// Visit all of the children.
	std::pair<TableDirectory::table_type, BaseTblPtr> first_statement = ctx->first_statement->accept(this);
	tbl_dir.register_tbl(first_statement);

	// Visit the rest of the children
	ctx->rest_of_statements->accept(this);
	return 0;
}

/**********************************************************************
	The methods below all return a pair of table_type and BaseTblPtr.
**********************************************************************/
antlrcpp::Any SQLComputeVisitor::visitStatementTypeSelect(SQLParser::StatementTypeSelectContext *ctx) {
	// All select statements will be in a vector of pairs of BaseTblPtr and union_type.
	std::vector<std::pair<BaseTblPtr, SQLComputeVisitor::union_type>> the_statement = ctx->the_statement->accept(this);
	BaseTblPtr result = unionHelper(the_statement);
	
	std::pair<TableDirectory::table_type, BaseTblPtr> result_pair(TableDirectory::SELECT, result);
	return result_pair;
}

antlrcpp::Any SQLComputeVisitor::visitStatementTypeCreate(SQLParser::StatementTypeCreateContext *ctx) {
	BaseTblPtr the_statement = ctx->the_statement->accept(this);
	std::pair<TableDirectory::table_type, BaseTblPtr> result_pair(TableDirectory::CREATE, the_statement);
	return result_pair;
}

antlrcpp::Any SQLComputeVisitor::visitStatementTypeDrop(SQLParser::StatementTypeDropContext *ctx) {
	std::pair<TableDirectory::table_type, BaseTblPtr> result_pair = ctx->the_statement->accept(this);
	return result_pair;
}

antlrcpp::Any SQLComputeVisitor::visitStatementTypeDropIfExists(SQLParser::StatementTypeDropIfExistsContext *ctx) {
	std::pair<TableDirectory::table_type, BaseTblPtr> result_pair = ctx->the_statement->accept(this);
	return result_pair;
}

antlrcpp::Any SQLComputeVisitor::visitStatementTypeInsert(SQLParser::StatementTypeInsertContext *ctx) {
	BaseTblPtr the_statement = ctx->the_statement->accept(this);
	std::pair<TableDirectory::table_type, BaseTblPtr> result_pair(TableDirectory::INSERT, the_statement);
	return result_pair;
}

/**************************************************
	The methods below all return a BaseTblPtr.
**************************************************/
antlrcpp::Any SQLComputeVisitor::visitStatementCreate(SQLParser::StatementCreateContext *ctx) {
	// we should recieve a vector of strings from the column clause, the Identifier can be
	// used directly as the name of the table
	std::string table_name = ctx->Identifier()->getSymbol()->getText();
	std::vector<std::string> col_names = ctx->column_clause->accept(this);

	if (tbl_dir.does_tbl_exist(table_name)) {
		std::cout << "The table already exists: " << table_name << std::endl;
		abort();
	}
	if (BaseTable::DISK_MODE == 0) {
		BaseTblPtr result = make_data_table(table_name, col_names);
		return result;
	} else if (BaseTable::DISK_MODE == 1) {
		std::vector<std::string> column_types {}; // placeholder - not used anywhere
		std::string generic_type ("string");
		for (auto ele : col_names) {
			column_types.push_back(generic_type);
		}
		std::string filename = table_name + ".table";
		BaseTblPtr result = make_disk_data_table(filename, col_names, column_types);
		return result;
	} else { // DISK_MODE must be set to some improper value!  It should only be 1 or 0
		std::cout << "ERROR - BaseTable::DISK_MODE is set to neither 0 or 1, invalid DISK_MODE" << std::endl;
		std::cout << "Closing program" << std::endl;
		abort();
	}
}

antlrcpp::Any SQLComputeVisitor::visitStatementInsertWithColumns(SQLParser::StatementInsertWithColumnsContext *ctx) {
	std::string table_name = ctx->Identifier()->getSymbol()->getText();
	std::vector<std::string> column_clause = ctx->column_clause->accept(this);
	std::vector<std::vector<std::string>> row_clause = ctx->row_clause->accept(this);
	
	if (!tbl_dir.does_tbl_exist(table_name)) {
		std::cout << "insert requested on table that does not exist.  Closing program" << std::endl;
		abort();
	} else {
		BaseTblPtr target_table = tbl_dir.get_tbl(table_name, 1);
		target_table->insert_data_with_cols(column_clause, row_clause);
		return target_table;
	}
}

antlrcpp::Any SQLComputeVisitor::visitStatementInsertWithoutColumns(SQLParser::StatementInsertWithoutColumnsContext *ctx) {
	std::string table_name = ctx->Identifier()->getSymbol()->getText();
	std::vector<std::vector<std::string>> row_clause = ctx->row_clause->accept(this);
	if(!tbl_dir.does_tbl_exist(table_name)) {
		std::cout << "insert requested on table that does not exist.  Closing program" << std::endl;
		abort();
	} else {
		BaseTblPtr target_table = tbl_dir.get_tbl(table_name, 1);
		target_table->insert_data_without_cols(row_clause);
		return target_table;
	}
}

antlrcpp::Any SQLComputeVisitor::visitSelectNoWhere(SQLParser::SelectNoWhereContext *ctx) {
	std::vector<std::vector<std::string>> column_list = ctx->column_list->accept(this);
	BaseTblPtr from_clause = ctx->from_clause->accept(this);

	std::vector<std::string> new_column_list;
	for (auto element: column_list) {
	    if (element.size() == 1) {
			new_column_list.push_back(element.at(0));
	    } else {
			new_column_list.push_back(element.at(1));
			RhoColumnOperator rho_col_op(from_clause, element);
			from_clause = rho_col_op.fabricate();
	    }
	}

	PiOperator pi_op(from_clause, new_column_list);
	BaseTblPtr pi = pi_op.fabricate();
	return pi;
}

antlrcpp::Any SQLComputeVisitor::visitSelectStarNoWhere(SQLParser::SelectStarNoWhereContext *ctx) {
	BaseTblPtr from_clause = ctx->from_clause->accept(this);
	PiOperator pi_op(from_clause, {"*"});
	BaseTblPtr pi = pi_op.fabricate();
	return pi;
}

antlrcpp::Any SQLComputeVisitor::visitSelectWhere(SQLParser::SelectWhereContext *ctx) {
	std::vector<std::vector<std::string>> column_list = ctx->column_list->accept(this);
	BaseTblPtr from_clause = ctx->from_clause->accept(this);
	SigmaBaseTblPtr where_clause = ctx->where_clause->accept(this);
	SigmaOperator sigma_op(from_clause, where_clause);
	BaseTblPtr sigma = sigma_op.fabricate();

	std::vector<std::string> new_column_list;
	for (auto element: column_list) {
	    if (element.size() == 1) {
			new_column_list.push_back(element.at(0));
	    } else {
			new_column_list.push_back(element.at(1));
			RhoColumnOperator rho_col_op(sigma, element);
			sigma = rho_col_op.fabricate();
	    }
	}

	PiOperator pi_op(sigma, new_column_list);
	BaseTblPtr pi = pi_op.fabricate();
	return pi;
}

antlrcpp::Any SQLComputeVisitor::visitSelectStarWhere(SQLParser::SelectStarWhereContext *ctx) {
	BaseTblPtr from_clause = ctx->from_clause->accept(this);

	SigmaBaseTblPtr where_clause = ctx->where_clause->accept(this);
	SigmaOperator sigma_op(from_clause, where_clause);
	BaseTblPtr sigma = sigma_op.fabricate();

	PiOperator pi_op(sigma, {"*"});
	BaseTblPtr pi = pi_op.fabricate();
	return pi;
}

antlrcpp::Any SQLComputeVisitor::visitStatementDropTable(SQLParser::StatementDropTableContext *ctx) {
	std::string table_name = ctx->Identifier()->getSymbol()->getText();

	// The grammar expects a pair<TableDirectory::table_type, BaseTblPtr> to be returned from
	// all statement types, then that pair is an argument for TableDirectory::register_tbl()
	// In the case of table_type DROP, this BaseTblPtr (called 'placeholder') will never be used
	BaseTblPtr placeholder_tbl;
	std::pair<TableDirectory::table_type, BaseTblPtr> result(TableDirectory::DROP, placeholder_tbl);

	if (!tbl_dir.does_tbl_exist(table_name)) {
		std::cout << "drop table requested on table that does not exist.  Program closing." << std::endl;
		abort();
	} else {
		tbl_dir.drop_table(table_name);
		return result;
	}
}

antlrcpp::Any SQLComputeVisitor::visitStatementDropIfExistsTable(SQLParser::StatementDropIfExistsTableContext *ctx) {
	std::string table_name = ctx->Identifier()->getSymbol()->getText();

	// The grammar expects a pair<TableDirectory::table_type, BaseTblPtr> to be returned from
	// all statement types, then that pair is an argument for TableDirectory::register_tbl()
	// In the case of table_type DROP, this BaseTblPtr (called 'placeholder') will never be used
	BaseTblPtr placeholder_tbl;
	std::pair<TableDirectory::table_type, BaseTblPtr> result(TableDirectory::DROP, placeholder_tbl);

	if (!tbl_dir.does_tbl_exist(table_name)) { // table doesn't exist, so do nothing
		return result;
	} else { // then the table exists
		tbl_dir.drop_table(table_name);
		return result;
	}
}

antlrcpp::Any SQLComputeVisitor::visitFromSpecSingle(SQLParser::FromSpecSingleContext *ctx) {
	return ctx->table_name->accept(this);
}

antlrcpp::Any SQLComputeVisitor::visitFromSpecNaturalJoin(SQLParser::FromSpecNaturalJoinContext *ctx) {
	BaseTblPtr first_table = ctx->first_table->accept(this);
	BaseTblPtr second_table = ctx->join_rhs->accept(this);

	JoinNaturalOperator nj_op(first_table, second_table);
	BaseTblPtr res = nj_op.fabricate();
	return res;
}

antlrcpp::Any SQLComputeVisitor::visitFromSpecCrossJoin(SQLParser::FromSpecCrossJoinContext *ctx) {
	BaseTblPtr first_table = ctx->first_table->accept(this);
	BaseTblPtr second_table = ctx->join_rhs->accept(this);

	JoinCrossOperator cr_op(first_table, second_table);
	BaseTblPtr res = cr_op.fabricate();
	return res;
}

antlrcpp::Any SQLComputeVisitor::visitTableSpecSingle(SQLParser::TableSpecSingleContext *ctx) {
	std::string table_name = ctx->Identifier()->getSymbol()->getText();

	if (!tbl_dir.does_tbl_exist(table_name)) {
		std::cout << "select requested on table that does not exist.  Program closing." << std::endl;
		abort();
	} else {
		BaseTblPtr result = tbl_dir.get_tbl(table_name, 0);
		return result;
	}
}

antlrcpp::Any SQLComputeVisitor::visitTableSpecParenthesized(SQLParser::TableSpecParenthesizedContext *ctx) {
	return ctx->from_clause->accept(this);
}

antlrcpp::Any SQLComputeVisitor::visitTableSpecNestedQuery(SQLParser::TableSpecNestedQueryContext *ctx) {
	std::vector<std::pair<BaseTblPtr, SQLComputeVisitor::union_type>> nested = ctx->nested->accept(this);
	return unionHelper(nested);
}

BaseTblPtr SQLComputeVisitor::unionHelper(std::vector<std::pair<BaseTblPtr, SQLComputeVisitor::union_type>> the_statement) {
	BaseTblPtr result;
	bool result_set = false;

	for (int i = 0; i < int(the_statement.size()); i++) {
		switch(the_statement.at(i).second) {
			case UNION: 
				{
					if (!result_set) {
						UnionOperator union_op(the_statement.at(i).first, the_statement.at(i+1).first);
						result = union_op.fabricate();
						result_set = true;
					} else {
						UnionOperator union_op(result, the_statement.at(i+1).first);
						result = union_op.fabricate();
					}
					break;
				}
			case UNIONALL: 	
				{
					if (!result_set) {
						UnionAllOperator union_all_op(the_statement.at(i).first, the_statement.at(i+1).first);
						result = union_all_op.fabricate();
						result_set = true;
					} else {
						UnionAllOperator union_all_op(result, the_statement.at(i+1).first);
						result = union_all_op.fabricate();
					}
					break;
				}
			case DONE: 	
				{
					if (!result_set) {
						result = the_statement.at(i).first;
					}
					break;
				}
			default: std::cout << "This should never run." << std::endl;
					 exit(0);
	    }
	}

	return result;
}

/**********************************************************************************
	The methods below all return a vector of a pair of BaseTblPtr and union_type.
**********************************************************************************/

antlrcpp::Any SQLComputeVisitor::visitUnionStatementSingle(SQLParser::UnionStatementSingleContext *ctx) {
	BaseTblPtr first_select = ctx->first_select->accept(this);
	std::pair<BaseTblPtr, union_type> pair(first_select, DONE);
	std::vector< std::pair<BaseTblPtr, union_type> > first_statement{pair};
	return first_statement;
}

antlrcpp::Any SQLComputeVisitor::visitUnionStatementUnionMulti(SQLParser::UnionStatementUnionMultiContext *ctx) {
	// Create a pair with the first_select and UNION. 
	BaseTblPtr first_select = ctx->first_select->accept(this);
	std::pair<BaseTblPtr, union_type> first_statement(first_select, UNION);

	// Add the pair to the front ot the rest of the select statements to be unioned. 
	std::vector<std::pair<BaseTblPtr, union_type>> rest_of_selects = ctx->rest_of_selects->accept(this);
	rest_of_selects.insert(rest_of_selects.begin(), first_statement);
	return rest_of_selects;
}

antlrcpp::Any SQLComputeVisitor::visitUnionStatementUnionAllMulti(SQLParser::UnionStatementUnionAllMultiContext *ctx) {
	// Create a pair with the first_select and UNIONALL. 
	BaseTblPtr first_select = ctx->first_select->accept(this);
	std::pair<BaseTblPtr, union_type> first_statement(first_select, UNIONALL);

	// Add the pair to the front ot the rest of the select statements to be unioned. 
	std::vector<std::pair<BaseTblPtr, union_type>> rest_of_selects = ctx->rest_of_selects->accept(this);
	rest_of_selects.insert(rest_of_selects.begin(), first_statement);
	return rest_of_selects;
}

/*****************************************************************
	The methods below all return a vector of vector of strings.
*****************************************************************/

antlrcpp::Any SQLComputeVisitor::visitInsertRowSpecSingle(SQLParser::InsertRowSpecSingleContext *ctx) {
	std::vector<std::string> first_row = ctx->first_row->accept(this);
	std::vector<std::vector<std::string>> rows{first_row};
	return rows;
}

antlrcpp::Any SQLComputeVisitor::visitInsertRowSpecMulti(SQLParser::InsertRowSpecMultiContext *ctx) {
	std::vector<std::string> first_row = ctx->first_row->accept(this);
	std::vector<std::vector<std::string>> rest_of_rows = ctx->rest_of_rows->accept(this);
	rest_of_rows.insert(rest_of_rows.begin(), first_row);
	return rest_of_rows;
}

antlrcpp::Any SQLComputeVisitor::visitSelectColumnSpecSingle(SQLParser::SelectColumnSpecSingleContext *ctx) {
	std::vector<std::string> first_column = ctx->first_column->accept(this);
	std::vector<std::vector<std::string>> column_list {first_column};
	return column_list;
}

antlrcpp::Any SQLComputeVisitor::visitSelectColumnSpecMulti(SQLParser::SelectColumnSpecMultiContext *ctx) {
	// Optimization: use a list instead of a vector
	std::vector<std::string> first_column = ctx->first_column->accept(this);
	std::vector<std::vector<std::string>> rest_of_columns = ctx->rest_of_columns->accept(this);
	rest_of_columns.insert(rest_of_columns.begin(), first_column);
	return rest_of_columns;
}

/*******************************************************
	The methods below all return a vector of strings.
*******************************************************/
antlrcpp::Any SQLComputeVisitor::visitCreateColumnSpecSingle(SQLParser::CreateColumnSpecSingleContext *ctx) {
	std::string first_col_name = ctx->first_column->accept(this);
	std::vector<std::string> col_names{first_col_name};
	return col_names;
}

antlrcpp::Any SQLComputeVisitor::visitCreateColumnSpecMulti(SQLParser::CreateColumnSpecMultiContext *ctx) {
	std::string first_column = ctx->first_column->accept(this);
	std::vector<std::string> rest_of_cols = ctx->rest_of_columns->accept(this);
	rest_of_cols.insert(rest_of_cols.begin(), first_column);
	return rest_of_cols;
}

antlrcpp::Any SQLComputeVisitor::visitColumnSpecSingle(SQLParser::ColumnSpecSingleContext *ctx) {
	std::string first_col_name = ctx->first_column->accept(this);
	std::vector<std::string> col_names{first_col_name};
	return col_names;
}

antlrcpp::Any SQLComputeVisitor::visitColumnSpecMulti(SQLParser::ColumnSpecMultiContext *ctx) {
	std::string first_column = ctx->first_column->accept(this);
	std::vector<std::string> rest_of_cols = ctx->rest_of_columns->accept(this);
	rest_of_cols.insert(rest_of_cols.begin(), first_column);
	return rest_of_cols;
}

antlrcpp::Any SQLComputeVisitor::visitInsertValueSpecSingle(SQLParser::InsertValueSpecSingleContext *ctx) {
	std::string first_value = ctx->first_value->accept(this);
	std::vector<std::string> values{first_value};
	return values;
}

antlrcpp::Any SQLComputeVisitor::visitInsertValueSpecMulti(SQLParser::InsertValueSpecMultiContext *ctx) {
	std::string first_value = ctx->first_value->accept(this);
	std::vector<std::string> rest_of_values = ctx->rest_of_values->accept(this);
	rest_of_values.insert(rest_of_values.begin(), first_value);
	return rest_of_values;
}

antlrcpp::Any SQLComputeVisitor::visitSelectColumnNameSingle(SQLParser::SelectColumnNameSingleContext *ctx) {
	std::string col_name = ctx->Identifier()->getSymbol()->getText();
	std::vector<std::string> col_name_vec {col_name};
	return col_name_vec;
}

antlrcpp::Any SQLComputeVisitor::visitSelectColumnNameRho(SQLParser::SelectColumnNameRhoContext *ctx) {
	std::string old_col_name = ctx->Identifier()->getSymbol()->getText();
	std::string new_col_name = ctx->new_col_name->accept(this);

	std::vector<std::string> old_and_new {old_col_name, new_col_name};
	return old_and_new;
}

/************************************************
	The methods below all return a string.
************************************************/
antlrcpp::Any SQLComputeVisitor::visitColumnNameSingle(SQLParser::ColumnNameSingleContext *ctx) {
	std::string val = ctx->Identifier()->getSymbol()->getText();
	return val;
}

antlrcpp::Any SQLComputeVisitor::visitInsertValueTerminalNumConstant(SQLParser::InsertValueTerminalNumConstantContext *ctx) {
	std::string val = ctx->NumValue()->getSymbol()->getText();
	return val;
}

antlrcpp::Any SQLComputeVisitor::visitInsertValueTerminalStringLiteral(SQLParser::InsertValueTerminalStringLiteralContext *ctx) {
	std::string val = ctx->StringLiteral()->getSymbol()->getText();
	val = val.substr(1, val.size() - 2);
	return val;
}

antlrcpp::Any SQLComputeVisitor::visitNewColumnName(SQLParser::NewColumnNameContext *ctx) {
	return ctx->Identifier()->getSymbol()->getText();
}

antlrcpp::Any SQLComputeVisitor::visitColumnType(SQLParser::ColumnTypeContext *ctx){
	return ctx->StringLiteral()->getSymbol()->getText();
}


/******************************************************
	The methods below all return a SigmaBaseTblPtr.
******************************************************/
antlrcpp::Any SQLComputeVisitor::visitWhereLogicalAnd(SQLParser::WhereLogicalAndContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaLogicalOperator logic_op("and", left, right);
	SigmaBaseTblPtr logic_op_ptr = logic_op.fabricate();
	return logic_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereLogicalOr(SQLParser::WhereLogicalOrContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaLogicalOperator logic_op("or", left, right);
	SigmaBaseTblPtr logic_op_ptr = logic_op.fabricate();
	return logic_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereComparisonLT(SQLParser::WhereComparisonLTContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaComparisonOperator comparison_op("<", left, right);
	SigmaBaseTblPtr comparison_op_ptr = comparison_op.fabricate();
	return comparison_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereComparisonGT(SQLParser::WhereComparisonGTContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaComparisonOperator comparison_op(">", left, right);
	SigmaBaseTblPtr comparison_op_ptr = comparison_op.fabricate();
	return comparison_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereComparisonET(SQLParser::WhereComparisonETContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaComparisonOperator comparison_op("=", left, right);
	SigmaBaseTblPtr comparison_op_ptr = comparison_op.fabricate();
	return comparison_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereComparisonLTE(SQLParser::WhereComparisonLTEContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaComparisonOperator comparison_op("<=", left, right);
	SigmaBaseTblPtr comparison_op_ptr = comparison_op.fabricate();
	return comparison_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereComparisonGTE(SQLParser::WhereComparisonGTEContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaComparisonOperator comparison_op(">=", left, right);
	SigmaBaseTblPtr comparison_op_ptr = comparison_op.fabricate();
	return comparison_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWherePlus(SQLParser::WherePlusContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaArithmeticOperator arithmetic_op("+", left, right);
	SigmaBaseTblPtr arithmetic_op_ptr = arithmetic_op.fabricate();
	return arithmetic_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereMinus(SQLParser::WhereMinusContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaArithmeticOperator arithmetic_op("-", left, right);
	SigmaBaseTblPtr arithmetic_op_ptr = arithmetic_op.fabricate();
	return arithmetic_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereMul(SQLParser::WhereMulContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaArithmeticOperator arithmetic_op("*", left, right);
	SigmaBaseTblPtr arithmetic_op_ptr = arithmetic_op.fabricate();
	return arithmetic_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereDiv(SQLParser::WhereDivContext *ctx) {
	SigmaBaseTblPtr left = ctx->left->accept(this);
	SigmaBaseTblPtr right = ctx->right->accept(this);

	SigmaArithmeticOperator arithmetic_op("/", left, right);
	SigmaBaseTblPtr arithmetic_op_ptr = arithmetic_op.fabricate();
	return arithmetic_op_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereTerminalIdentifier(SQLParser::WhereTerminalIdentifierContext *ctx) {
	std::string val = ctx->Identifier()->getSymbol()->getText();
	SigmaColumnValueOperator value(val);
	SigmaBaseTblPtr value_ptr = value.fabricate();
	return value_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereTerminalNumConstant(SQLParser::WhereTerminalNumConstantContext *ctx) {
	std::string val = ctx->NumValue()->getSymbol()->getText();
	SigmaConstantValueOperator value(val);
	SigmaBaseTblPtr value_ptr = value.fabricate();
	return value_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereTerminalStringLiteral(SQLParser::WhereTerminalStringLiteralContext *ctx) {
	std::string val = ctx->StringLiteral()->getSymbol()->getText();
	val = val.substr(1, val.size() - 2);
	SigmaConstantValueOperator value(val);
	SigmaBaseTblPtr value_ptr = value.fabricate();
	return value_ptr;
}

antlrcpp::Any SQLComputeVisitor::visitWhereTerminalNestedExpr(SQLParser::WhereTerminalNestedExprContext *ctx) {
	return ctx->where_logical_expr()->accept(this);	
}

