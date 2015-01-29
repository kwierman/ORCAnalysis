#ifndef Deuterium_DataFormat_DataTable_hh_
#define Deuterium_DataFormat_DataTable_hh_

namespace Deuterium{
	namespace DataFormat{

		/** A class for containing the entry in the table, it needs to point to the Column and 
		**/
		class TableEntry : public DataNode{
		public:
			//
			TableEntry(const std::string& value );
			void Accept(const Table& tab);
		};

		class TableColumn : DataNode{
		public:
			TableColumn(const std::string& col);

		};

		class TableRow : DataNode{
		public:
			TableRow(const std::string& row);
		};


		/** A table is a data node in which the children are rows, columns and 
		**/
		class Table{
		public:
			void SetDefault(const TableEntry& entry);
			void Insert(const TableColumn& col);
			void Insert(const TableRow& row);
			void Insert(const TableColumn& col, const TableRow& row, const TableEntry& entry);
			void Insert(const std::string& col, const std::string& row, const TableEntry& entry);


		};
	}
}



#endif //File Guardian