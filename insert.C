#include "catalog.h"
#include "query.h"


/*
 * Inserts a record into the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Insert(const string & relation, 
	const int attrCnt, 
	const attrInfo attrList[])
{
 int relAttrCnt;
    AttrDesc* relAttrList;
	Status status;
    status = attrCat->getRelInfo(relation, relAttrCnt, relAttrList);
    if (status != OK) {
        return status;
    }

   
    int recordSize = 0;
    for (int i = 0; i < relAttrCnt; i++) {
        recordSize += relAttrList[i].attrLen;
    }

   
    void* recordBuffer = new char[recordSize];
    memset(recordBuffer, 0, recordSize);

    
    for (int i = 0; i < attrCnt; i++) {
        for (int j = 0; j < relAttrCnt; j++) {
            if (attrList[i].attrName == relAttrList[j].attrName) {
                if (attrList[i].attrValue == nullptr) {
                    delete[] relAttrList;
                    delete[] recordBuffer;
                    // return  ???; // Reject the insertion due to NULL value
                }
                memcpy((char*)recordBuffer + relAttrList[j].attrOffset, attrList[i].attrValue, relAttrList[j].attrLen);
                break;
            }
        }
    }

	Record rec;
	rec.data=recordBuffer;
	rec.length=recordSize;

	RID outRid;
    InsertFileScan insertFileScan(relation, status);
    if (status != OK) {
        delete[] relAttrList;
        delete[] recordBuffer;
        return status;
    }
    status = insertFileScan.insertRecord(rec,outRid);
    if (status != OK) {
        delete[] relAttrList;
        delete[] recordBuffer;
        return status;
    }

    // Clean up and return
    delete[] relAttrList;
    delete[] recordBuffer;
   

return OK;

}

