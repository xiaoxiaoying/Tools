#include <string>
#include <map>
#include <memory>
#include <vector>
using namespace std;

#include "../include/CommonException.h"
#include "../include/StringFacility.h"
using namespace hudun::common;

#include "../include/ColValue.h"
using namespace hudun::sqlite;

#include "../include/ResultSet.h"
#include "../include/Strto.h"

using namespace hudun::sqlite::baseapi;

uint32_t ResultSet::getColsNum() const noexcept
{
    return (uint32_t)this->colNames.size();
}

uint32_t ResultSet::getRowsNum() const noexcept
{
    return (uint32_t)this->rows.size();
}

const vector<string>& ResultSet::getColNames() const
{
    return this->colNames;
}

string ResultSet::getColName(uint32_t colIndex) const
{
    if(colIndex >= this->colNames.size())
    {
        throw CommonException(CommonException::FAIL_GENERAL, "colIndex[" + to_string(colIndex) + "] overflow[" + to_string(this->colNames.size()) + "]!", __FILE__, __func__, __LINE__);
    }
    return this->colNames[colIndex];
}

const vector<shared_ptr<ColValue>>& ResultSet::getRow(uint32_t rowIndex) const
{
    if(rowIndex >= this->rows.size())
    {
        throw CommonException(CommonException::FAIL_GENERAL, "rowIndex[" + to_string(rowIndex) + "] overflow[" + to_string(this->rows.size()) + "]!", __FILE__, __func__, __LINE__);
    }
    return this->rows[rowIndex];
}

shared_ptr<ColValue> ResultSet::getValue(uint32_t rowIndex, uint32_t colIndex) const
{
    const vector<shared_ptr<ColValue>>& row = this->getRow(rowIndex);
    if(colIndex >= this->colNames.size())
    {
        throw CommonException(CommonException::FAIL_GENERAL, "colIndex[" + to_string(colIndex) + "] overflow[" + to_string(this->colNames.size()) + "]!", __FILE__, __func__, __LINE__);
    }
    return row[colIndex];
}

shared_ptr<ColValue> ResultSet::getValue(uint32_t rowIndex, string const& colName) const
{
    string capColName = colName;
    StringFacility::toupper(capColName);
    map<string, uint32_t>::const_iterator it = this->capColNameToIndexMap.find(capColName);
    if(it == this->capColNameToIndexMap.end())
    {
        throw CommonException(CommonException::FAIL_GENERAL, "colName[" + colName + "] not exist!", __FILE__, __func__, __LINE__);
    }

    return this->getValue(rowIndex, it->second);
}

vector<shared_ptr<ColValue>> ResultSet::listColValues(uint32_t colIndex) const
{
    if(colIndex >= this->colNames.size())
    {
        throw CommonException(CommonException::FAIL_GENERAL, "colIndex[" + to_string(colIndex) + "] overflow[" + to_string(this->colNames.size()) + "]!", __FILE__, __func__, __LINE__);
    }

    vector<shared_ptr<ColValue>> colValues;
    for(uint32_t i = 0; i < this->rows.size(); i ++)
    {
        colValues.push_back(this->rows[i][colIndex]);
    }

    return colValues;
}

vector<shared_ptr<ColValue>> ResultSet::listColValues(string const& colName) const
{
    string capColName = colName;
    StringFacility::toupper(capColName);
    map<string, uint32_t>::const_iterator it = this->capColNameToIndexMap.find(capColName);
    if(it == this->capColNameToIndexMap.end())
    {
        throw CommonException(CommonException::FAIL_GENERAL, "colName[" + colName + "] not exist!", __FILE__, __func__, __LINE__);
    }
    return this->listColValues(it->second);
}

void ResultSet::clear() noexcept
{
    this->capColNameToIndexMap.clear();
    this->colNames.clear();
    this->rows.clear();
}

void ResultSet::displayColsNum(ostream& out) const noexcept
{
    // colNames;
    for(uint32_t colIndex = 0; colIndex < this->colNames.size(); colIndex ++)
    {
        if(colIndex > 0) out << "\t";
        out << this->colNames[colIndex];
    }
    out << endl;
}

void ResultSet::displayRow(ostream& out, vector<shared_ptr<ColValue>> const& row, DisplayFlag displayFlag) const noexcept
{
    if((displayFlag & DisplayFlag::HORIZONTAL) == DisplayFlag::HORIZONTAL)
    {
        for(uint32_t colIndex = 0; colIndex < this->colNames.size(); colIndex ++)
        {
            if(colIndex > 0) out << "\t";
            out << *(row[colIndex]);
        }
    }
    else
    {
        for(uint32_t colIndex = 0; colIndex < this->colNames.size(); colIndex ++)
        {
            out << this->colNames[colIndex] << ": " << *(row[colIndex]) << endl;
        }
    }
    out << endl;
}

void ResultSet::display(ostream& out, DisplayFlag displayFlag) const noexcept
{
    if((displayFlag & DisplayFlag::HORIZONTAL) == DisplayFlag::HORIZONTAL)
    {
        displayColsNum(out);
    }

    for(uint32_t rowIndex = 0; rowIndex < this->rows.size(); rowIndex ++)
    {
        const vector<shared_ptr<ColValue>>& row = getRow(rowIndex);
        displayRow(out, row, displayFlag);
    }
}
