/*
 * CSVReader.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: grigorii
 */

#include "CSVReader.hpp"
#include <sqlite3.h>
extern Database *db;

CSVReader::CSVReader(string fileContent,char* dataBaseName) {
	CSVReader::fileContent = fileContent;
	CSVReader::dataBaseName = dataBaseName;

}

string CSVReader::getfileContent() {
	return CSVReader::fileContent;
}

char* CSVReader::getDataBaseName(){
	return CSVReader::dataBaseName;
}

void CSVReader::readAll(string collection_name) {
	string line;

	vector<string> fields;
	string infohash;

	stringstream mydata;
	mydata<<fileContent;
//	Database *db = new Database();

		sqlite3 *db_exec;
		char *zErrMsg = 0;
		int rc;

		int i = 0;

		rc = sqlite3_open_v2(DATABASE_NAME, &db_exec, SQLITE_OPEN_READWRITE, NULL);

		if(rc == SQLITE_OK){
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
					string q="INSERT  OR IGNORE INTO Torrents (type, name,infohash,sizebytes,mirrors,downloaders,timescompleted,dateadded,datemodified) VALUES (";
					for (size_t j=0;j<fields.size()-1;j++)
						if (j<3 || j>6 )
						q+="'"+fields[j]+"'"+",";
						else
						q+=fields[j]+",";

					q+=fields[fields.size()-1]+");";
					//db->open(dataBaseName);
					int retval;
					bool continueTrying = true;
					 while (continueTrying) {
					    retval = sqlite3_exec(db_exec, &q[0], NULL, 0, &zErrMsg);
					    switch (retval) {
					      case SQLITE_BUSY:
					       // Log("[%s] SQLITE_BUSY: sleeping fow a while...", threadName);
					        sleep(3000);
					        break;
					      case SQLITE_OK:
					        continueTrying = false; // We're done
					        break;
					      default:
					        //Log("[%s] Can't execute \"%s\": %s\n", threadName, sqlQuery, msg);
					        continueTrying = false;
					        break;
					    }
					  }
					 continueTrying = true;
					 string q2 = "INSERT OR IGNORE INTO Collections2Torrents VALUES(\'" + fields[2] +"\', \'"+ collection_name + "\');";
					 while (continueTrying) {
					    retval = sqlite3_exec(db_exec, &q2[0], NULL, 0, &zErrMsg);
					    switch (retval) {
					      case SQLITE_BUSY:
					        //Log("[%s] SQLITE_BUSY: sleeping fow a while...", threadName);
					        sleep(3000);
					        break;
					      case SQLITE_OK:
					        continueTrying = false; // We're done
					        break;
					      default:
					        //Log("[%s] Can't execute \"%s\": %s\n", threadName, sqlQuery, msg);
					        continueTrying = false;
					        break;
					    }
					  }

					//db->query(&q[0]);
					//db->query("INSERT OR IGNORE INTO Collections2Torrents VALUES(\'" + fields[2] +"\', \'"+ collection_name + "\');");
					//db->close();
				}
				//update association to collection
				i++;
			}
		} else {
			cout << "error opening database" << endl;
		}
        // //db->close();

}

void CSVReader::readCollections(){
	string line;

	sqlite3 *db_exec;
		char *zErrMsg = 0;
		int rc;

		vector<string> fields;
	//
		stringstream mydata;
		mydata<<fileContent;
//		Database *db = new Database();
		

			int i = 0;


			rc = sqlite3_open_v2(DATABASE_NAME, &db_exec, SQLITE_OPEN_READWRITE, NULL);
			if(rc == SQLITE_OK){

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
					//db->open(dataBaseName);
					bool continueTrying = true;
					int retval;
					while (continueTrying) {
		     		    retval = sqlite3_exec(db_exec,&q[0], NULL, 0, &zErrMsg);
		     		    switch (retval) {
		     		    case SQLITE_BUSY:
		     		    	//Log("[%s] SQLITE_BUSY: sleeping fow a while...", threadName);
						  sleep(3000);
						  break;
						  case SQLITE_OK:
						  continueTrying = false; // We're done
						  break;
						  default:
						  //Log("[%s] Can't execute \"%s\": %s\n", threadName, sqlQuery, msg);
						  continueTrying = false;
						  break;
					}
//					db->query(&q[0]);
					//db->close();
				}
				i++;
					}
				}
			}
	        // //db->close();
}
CSVReader::~CSVReader() {}
