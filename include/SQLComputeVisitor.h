/*  SQLComputeVisitor.h
    Adapted from B. Bird - 10/24/2018
*/

#ifndef SQLCOMPUTEVISITOR_H
#define SQLCOMPUTEVISITOR_H


#include <iostream>
#include <string>
#include <unistd.h>

//Need to add a -I with the path to antlr headers
//#include "antlr4-runtime.h"

//Need -I./antlr_output/
#include "SQLBaseVisitor.h"

#include "DataTable.h"
#include "BaseTable.h"
#include "TableDirectory.h"
#include "DiskDataTable.h"

#include "PiOperator.h"
#include "UnionOperator.h"
#include "SigmaOperator.h"
#include "UnionAllOperator.h"
#include "RhoTableOperator.h"
#include "RhoColumnOperator.h"
#include "JoinCrossOperator.h"
#include "JoinNaturalOperator.h"

#include "SigmaBaseOperator.h"
#include "SigmaLogicalOperator.h"
#include "SigmaArithmeticOperator.h"
#include "SigmaComparisonOperator.h"
#include "SigmaColumnValueOperator.h"
#include "SigmaConstantValueOperator.h"


using std::string;


class  SQLComputeVisitor : public SQLBaseVisitor {

private:
	// tbl_dir is a global variable used to gain access to member functionsin TableDirectory.
	// TODO: remove the class definition from TableDirectory and just use the functions
	// and the two global variables (created_tables and results_tables) directly
	TableDirectory tbl_dir;
	enum union_type {UNION, UNIONALL, DONE};
	
public:

/********************************************************
	The methods below all return a vector of BaseTblPtr.
********************************************************/

	virtual antlrcpp::Any visitPrimary_expression(SQLParser::Primary_expressionContext *ctx) override;

/*************************************************
	The method below returns a placeholder 0.
*************************************************/

	virtual antlrcpp::Any visitStatementSingle(SQLParser::StatementSingleContext *ctx) override;

	virtual antlrcpp::Any visitStatementMultiple(SQLParser::StatementMultipleContext *ctx) override;

/**********************************************************************
	The methods below all return a pair of table_type and BaseTblPtr.
**********************************************************************/

	virtual antlrcpp::Any visitStatementTypeSelect(SQLParser::StatementTypeSelectContext *ctx) override;

	virtual antlrcpp::Any visitStatementTypeCreate(SQLParser::StatementTypeCreateContext *ctx) override;

	virtual antlrcpp::Any visitStatementTypeDrop(SQLParser::StatementTypeDropContext *ctx) override;

	virtual antlrcpp::Any visitStatementTypeDropIfExists(SQLParser::StatementTypeDropIfExistsContext *ctx) override;
	
	virtual antlrcpp::Any visitStatementTypeInsert(SQLParser::StatementTypeInsertContext *ctx) override;

/**************************************************
	The methods below all return a BaseTblPtr.
**************************************************/

	virtual antlrcpp::Any visitStatementCreate(SQLParser::StatementCreateContext *ctx) override;

	virtual antlrcpp::Any visitStatementInsertWithColumns(SQLParser::StatementInsertWithColumnsContext *ctx) override;

	virtual antlrcpp::Any visitStatementInsertWithoutColumns(SQLParser::StatementInsertWithoutColumnsContext *ctx) override;

	virtual antlrcpp::Any visitSelectNoWhere(SQLParser::SelectNoWhereContext *ctx) override;

	virtual antlrcpp::Any visitSelectStarNoWhere(SQLParser::SelectStarNoWhereContext *ctx) override;

	virtual antlrcpp::Any visitSelectWhere(SQLParser::SelectWhereContext *ctx) override;

	virtual antlrcpp::Any visitSelectStarWhere(SQLParser::SelectStarWhereContext *ctx) override;

	virtual antlrcpp::Any visitStatementDropTable(SQLParser::StatementDropTableContext *ctx) override;

	virtual antlrcpp::Any visitStatementDropIfExistsTable(SQLParser::StatementDropIfExistsTableContext *ctx) override;

	virtual antlrcpp::Any visitFromSpecSingle(SQLParser::FromSpecSingleContext *ctx) override;

	virtual antlrcpp::Any visitFromSpecNaturalJoin(SQLParser::FromSpecNaturalJoinContext *ctx) override;

	virtual antlrcpp::Any visitFromSpecCrossJoin(SQLParser::FromSpecCrossJoinContext *ctx) override;

	virtual antlrcpp::Any visitTableSpecSingle(SQLParser::TableSpecSingleContext *ctx) override;

    virtual antlrcpp::Any visitTableSpecParenthesized(SQLParser::TableSpecParenthesizedContext *ctx) override;

	virtual antlrcpp::Any visitTableSpecNestedQuery(SQLParser::TableSpecNestedQueryContext *ctx) override;

	BaseTblPtr unionHelper(std::vector<std::pair<BaseTblPtr, SQLComputeVisitor::union_type>> the_statement);

/**********************************************************************************
	The methods below all return a vector of a pair of BaseTblPtr and union_type.
**********************************************************************************/

	virtual antlrcpp::Any visitUnionStatementSingle(SQLParser::UnionStatementSingleContext *ctx) override;

	virtual antlrcpp::Any visitUnionStatementUnionMulti(SQLParser::UnionStatementUnionMultiContext *ctx) override;

	virtual antlrcpp::Any visitUnionStatementUnionAllMulti(SQLParser::UnionStatementUnionAllMultiContext *ctx) override;

/*****************************************************************
	The methods below all return a vector of vector of strings.
*****************************************************************/

	virtual antlrcpp::Any visitInsertRowSpecSingle(SQLParser::InsertRowSpecSingleContext *ctx) override;

	virtual antlrcpp::Any visitInsertRowSpecMulti(SQLParser::InsertRowSpecMultiContext *ctx) override;

	virtual antlrcpp::Any visitSelectColumnSpecSingle(SQLParser::SelectColumnSpecSingleContext *ctx) override;

	virtual antlrcpp::Any visitSelectColumnSpecMulti(SQLParser::SelectColumnSpecMultiContext *ctx) override;
	
/*******************************************************
	The methods below all return a vector of strings.
*******************************************************/

	virtual antlrcpp::Any visitCreateColumnSpecSingle(SQLParser::CreateColumnSpecSingleContext *ctx) override;

	virtual antlrcpp::Any visitCreateColumnSpecMulti(SQLParser::CreateColumnSpecMultiContext *ctx) override;

	virtual antlrcpp::Any visitColumnSpecSingle(SQLParser::ColumnSpecSingleContext *ctx) override;

	virtual antlrcpp::Any visitColumnSpecMulti(SQLParser::ColumnSpecMultiContext *ctx) override;

	virtual antlrcpp::Any visitInsertValueSpecSingle(SQLParser::InsertValueSpecSingleContext *ctx) override;

	virtual antlrcpp::Any visitInsertValueSpecMulti(SQLParser::InsertValueSpecMultiContext *ctx) override;

	virtual antlrcpp::Any visitSelectColumnNameSingle(SQLParser::SelectColumnNameSingleContext *ctx) override;

	virtual antlrcpp::Any visitSelectColumnNameRho(SQLParser::SelectColumnNameRhoContext *ctx) override;

/************************************************
	The methods below all return a string.
************************************************/

	virtual antlrcpp::Any visitColumnNameSingle(SQLParser::ColumnNameSingleContext *ctx) override;

	virtual antlrcpp::Any visitInsertValueTerminalNumConstant(SQLParser::InsertValueTerminalNumConstantContext *ctx) override;

	virtual antlrcpp::Any visitInsertValueTerminalStringLiteral(SQLParser::InsertValueTerminalStringLiteralContext *ctx) override;

	virtual antlrcpp::Any visitNewColumnName(SQLParser::NewColumnNameContext *ctx) override;

    virtual antlrcpp::Any visitColumnType(SQLParser::ColumnTypeContext *context) override;

/******************************************************
	The methods below all return a SigmaBaseTblPtr.
******************************************************/

	virtual antlrcpp::Any visitWhereLogicalAnd(SQLParser::WhereLogicalAndContext *ctx) override;

	virtual antlrcpp::Any visitWhereLogicalOr(SQLParser::WhereLogicalOrContext *ctx) override;

	virtual antlrcpp::Any visitWhereComparisonLT(SQLParser::WhereComparisonLTContext *ctx) override;

	virtual antlrcpp::Any visitWhereComparisonGT(SQLParser::WhereComparisonGTContext *ctx) override;

	virtual antlrcpp::Any visitWhereComparisonET(SQLParser::WhereComparisonETContext *ctx) override;

	virtual antlrcpp::Any visitWhereComparisonLTE(SQLParser::WhereComparisonLTEContext *ctx) override;

	virtual antlrcpp::Any visitWhereComparisonGTE(SQLParser::WhereComparisonGTEContext *ctx) override;

	virtual antlrcpp::Any visitWherePlus(SQLParser::WherePlusContext *ctx) override;

	virtual antlrcpp::Any visitWhereMinus(SQLParser::WhereMinusContext *ctx) override;

	virtual antlrcpp::Any visitWhereMul(SQLParser::WhereMulContext *ctx) override;

	virtual antlrcpp::Any visitWhereDiv(SQLParser::WhereDivContext *ctx) override;

	virtual antlrcpp::Any visitWhereTerminalIdentifier(SQLParser::WhereTerminalIdentifierContext *ctx) override;

	virtual antlrcpp::Any visitWhereTerminalNumConstant(SQLParser::WhereTerminalNumConstantContext *ctx) override;

	virtual antlrcpp::Any visitWhereTerminalStringLiteral(SQLParser::WhereTerminalStringLiteralContext *ctx) override;

	virtual antlrcpp::Any visitWhereTerminalNestedExpr(SQLParser::WhereTerminalNestedExprContext *ctx) override;

};

#endif
