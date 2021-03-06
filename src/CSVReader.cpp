/*
 * CSVReader.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: grigorii
 */

#include "CSVReader.hpp"


CSVReader::CSVReader(string fileContent) {
	CSVReader::fileContent = fileContent;
//	CSVReader::dataBaseName = dataBaseName;

}

string CSVReader::getfileContent() {
	return CSVReader::fileContent;
}

//char* CSVReader::getDataBaseName(){
//	return CSVReader::dataBaseName;
//}

void CSVReader::readAll(string collection_name) {
	string line;

	vector<string> fields;
	string infohash;

	stringstream mydata;
	mydata<<fileContent;
<<<<<<< HEAD
	Database *db;
	db = new Database();

 //   db->query(TORRENTS_TABLE);
=======
	Database *db = new Database();

	db->open(dataBaseName);
>>>>>>> server-build

		int i = 0;

		while (getline(mydata, line)) {
			if (i != 0) {
//				boost::split(fields, line, boost::is_any_of(","));
				//TODO REMOVE TEST CODE
				boost::replace_all(line, "\'", "\'\'");
				int start=0,j=0;
				            fields.clear();
				            do
				            {
				             if (line[j]=='"')
				            	while(line[++j]!='"');
				             if (line[j]==',')
				              {
				            	fields.push_back(line.substr(start,j-start));
				            	start=j+1;
				               }
				            }
				            while(j++!=line.length());
				            fields.push_back(line.substr(start,j-start-1));
				/* TorrentInfo tor(fields); */
				/* adding a torrent info to the data base */
//				infohash
				string q="INSERT OR REPLACE INTO Torrents (type, name,infohash,sizebytes,mirrors,downloaders,timescompleted,dateadded,datemodified) VALUES (";
				for (size_t j=0;j<fields.size()-1;j++)
					if (j<3 || j>6 )
					q+="'"+fields[j]+"'"+",";
					else
					q+=fields[j]+",";

				q+=fields[fields.size()-1]+");";
				db->query(&q[0]);
				db->query("INSERT OR REPLACE INTO Collections2Torrents VALUES(\"" + fields[2] +"\", \'"+ collection_name + "\');");
			}
			//update association to collection
			i++;
		}

        db->close();

}

void CSVReader::readCollections(){
	string line;

		vector<string> fields;
	//
		stringstream mydata;
		mydata<<fileContent;
		Database *db = new Database();
		db->open(dataBaseName);

	    db->query(TORRENTS_TABLE);

			int i = 0;

			while (getline(mydata, line)) {
				if (i != 0) {
					boost::split(fields, line, boost::is_any_of(","));
					//TorrentInfo tor(fields);
					//adding a torrent info to the data base
					string q="INSERT OR IGNORE INTO Collections VALUES(";
					for (size_t j=0;j<fields.size();j++)
						if (j<3 || j>6)
						q+=""+fields[j]+""+",";
						else
						q+=fields[j];

					q+=",0);";
					//q.replace(q.length()-1,2,");");
					db->query(&q[0]);
				}
				i++;
			}

	        db->close();
}
CSVReader::~CSVReader() {}

