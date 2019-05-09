grammar SQL;

PLUS:		'+';
MINUS:		'-';
MUL:		'*';
DIV:		'/';

LEFT_PAREN:	'(';
RIGHT_PAREN:	')';

CREATE:		'create table';
INSERT:		'insert into';
VALUES:		'values';
DROP:		'drop table';
DROP_IF_EXISTS:	'drop table if exists';
SELECT:		'select';

FROM:		'from';
WHERE:		'where';
NATURALJOIN:	'natural join';
CROSSJOIN:	'cross join';
AS:		'as';
UNION:		'union';
UNIONALL:	'union all';

COMMA:		',';
SEMICOLON:	';';

AND:		'and';
OR:		'or';

LT:		'<';
GT:		'>';
ET:		'=';
LTE:		'<=';
GTE:		'>=';

Identifier:	[a-zA-Z][a-zA-Z_.0-9]*; // Table names, column names, etc.
NumValue:	[0-9]+(.[0-9][0-9]*)?;
StringLiteral:	'\'' .*? '\''; // Does not accommodate literals with escaped quotes

WHITESPACE:	[ \t\r\n]+ -> skip;
LINE_COMMENT:	'--' ~[\r\n]* -> skip;

primary_expression	: statement SEMICOLON EOF
			;

statement		: first_statement=statement_type									# StatementSingle
			| first_statement=statement_type SEMICOLON rest_of_statements=statement					# StatementMultiple
			;

statement_type		: the_statement=union_statement										# StatementTypeSelect
			| the_statement=create_statement									# StatementTypeCreate
			| the_statement=insert_statement									# StatementTypeInsert
			| the_statement=drop_statement										# StatementTypeDrop
			| the_statement=drop_if_exists_statement								# StatementTypeDropIfExists
			;


create_statement	: CREATE Identifier LEFT_PAREN column_clause=create_column_spec RIGHT_PAREN				# StatementCreate
			;

create_column_spec	: first_column=column_name first_type=column_type							# CreateColumnSpecSingle
			| first_column=column_name first_type=column_type COMMA rest_of_columns=create_column_spec		# CreateColumnSpecMulti
			;
				 
column_type		: StringLiteral												# ColumnType
			;

column_spec		: first_column=column_name										# ColumnSpecSingle
			| first_column=column_name COMMA rest_of_columns=column_spec						# ColumnSpecMulti
			;

column_name		: Identifier												# ColumnNameSingle
			;

insert_statement	: INSERT Identifier LEFT_PAREN column_clause=column_spec RIGHT_PAREN VALUES row_clause=insert_row_spec	# StatementInsertWithColumns
			| INSERT Identifier VALUES row_clause=insert_row_spec							# StatementInsertWithoutColumns
			;

insert_row_spec		: LEFT_PAREN first_row=insert_value_spec RIGHT_PAREN							# InsertRowSpecSingle
			| LEFT_PAREN first_row=insert_value_spec RIGHT_PAREN COMMA rest_of_rows=insert_row_spec			# InsertRowSpecMulti
			;

insert_value_spec	: first_value=insert_value_terminal									# InsertValueSpecSingle
			| first_value=insert_value_terminal COMMA rest_of_values=insert_value_spec				# InsertValueSpecMulti
			;

insert_value_terminal	: NumValue												# InsertValueTerminalNumConstant
			| StringLiteral												# InsertValueTerminalStringLiteral
			;

union_statement		: first_select=select_statement										# UnionStatementSingle
			| first_select=select_statement UNION rest_of_selects=union_statement					# UnionStatementUnionMulti
			| first_select=select_statement UNIONALL rest_of_selects=union_statement				# UnionStatementUnionAllMulti
			;

select_statement	: SELECT column_list=select_column_spec FROM from_clause=from_spec					# SelectNoWhere
			| SELECT MUL FROM from_clause=from_spec									# SelectStarNoWhere
			| SELECT column_list=select_column_spec FROM from_clause=from_spec WHERE where_clause=where_spec	# SelectWhere
			| SELECT MUL FROM from_clause=from_spec WHERE where_clause=where_spec					# SelectStarWhere
			;

select_column_spec	: first_column=select_column_name									# SelectColumnSpecSingle
			| first_column=select_column_name COMMA rest_of_columns=select_column_spec				# SelectColumnSpecMulti
			;

drop_statement		: DROP Identifier											# StatementDropTable
			;

drop_if_exists_statement: DROP_IF_EXISTS Identifier										# StatementDropIfExistsTable
			;

select_column_name	: Identifier												# SelectColumnNameSingle
			| Identifier AS new_col_name=new_column_name								# SelectColumnNameRho
			;

new_column_name		: Identifier												# NewColumnName
			;

from_spec		: table_name=table_spec											# FromSpecSingle
			| first_table=table_spec NATURALJOIN join_rhs=from_spec							# FromSpecNaturalJoin
			| first_table=table_spec CROSSJOIN join_rhs=from_spec							# FromSpecCrossJoin
			;

table_spec		: Identifier												# TableSpecSingle
			| LEFT_PAREN from_clause=from_spec RIGHT_PAREN								# TableSpecParenthesized
			| LEFT_PAREN nested=union_statement RIGHT_PAREN								# TableSpecNestedQuery
			;

where_spec		: where_logical_expr
			;

where_logical_expr	: where_comparison_expr											# WhereLogicalPassthrough
			| left=where_comparison_expr AND right=where_logical_expr						# WhereLogicalAnd
			| left=where_comparison_expr OR right=where_logical_expr						# WhereLogicalOr
			;

//Note that this does not allow comparisons to be chained like logical operations
//(e.g. "x < y < z" doesn't make any sense)
where_comparison_expr	: where_addsub_expr											# WhereComparisonPassthrough
			| left=where_addsub_expr LT right=where_addsub_expr							# WhereComparisonLT
			| left=where_addsub_expr GT right=where_addsub_expr							# WhereComparisonGT
			| left=where_addsub_expr ET right=where_addsub_expr							# WhereComparisonET
			| left=where_addsub_expr LTE right=where_addsub_expr							# WhereComparisonLTE
			| left=where_addsub_expr GTE right=where_addsub_expr							# WhereComparisonGTE
			;

where_addsub_expr	: where_muldiv_expr											# WhereAddPassthrough
			| left=where_muldiv_expr PLUS right=where_addsub_expr							# WherePlus
			| left=where_muldiv_expr MINUS right=where_addsub_expr							# WhereMinus
			;

where_muldiv_expr	: where_terminal											# WhereMulPassthrough
			| left=where_terminal MUL right=where_muldiv_expr							# WhereMul
			| left=where_terminal DIV right=where_muldiv_expr							# WhereDiv
			;

where_terminal		: Identifier												# WhereTerminalIdentifier
			| NumValue												# WhereTerminalNumConstant
			| StringLiteral												# WhereTerminalStringLiteral
			| LEFT_PAREN where_logical_expr RIGHT_PAREN								# WhereTerminalNestedExpr
			;
