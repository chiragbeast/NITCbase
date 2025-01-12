#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include<iostream>
#include<cstring>
void makefile(){
  unsigned char buffer1[BLOCK_SIZE];


for(int i = 0 ; i<3 ;i++){
            Disk::readBlock(buffer1, 7000);
            char message1[100] ;
            std::cin>>message1;


  memcpy(buffer1 + 20, message1, sizeof(message1));
  Disk::writeBlock(buffer1, 7000);
  unsigned char buffer4[BLOCK_SIZE];

  char message4[100];
  Disk::readBlock(buffer4, 7000);
  memcpy(message4, buffer4 + 20, sizeof(message4));

  std::cout << message4;
      
}
 
  

  
}
void printRelation(){
     RecBuffer relCatBuffer(RELCAT_BLOCK);
     RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

    HeadInfo relCatHeader;
    HeadInfo attrCatHeader;

    // Load the headers of both the blocks into relCatHeader and attrCatHeader
    relCatBuffer.getHeader(&relCatHeader);
    attrCatBuffer.getHeader(&attrCatHeader);
    

    for (int i = 0,attrcount = 0 ; i < relCatHeader.numEntries; ++i) {
        Attribute relCatRecord[RELCAT_NO_ATTRS]; // To store the record from the relation catalog

        relCatBuffer.getRecord(relCatRecord, i);

       std::cout<< "Relation: "<< relCatRecord[RELCAT_REL_NAME_INDEX].sVal<<std::endl;
        
       int j = 0 ;
       for(; j<relCatRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal ; j++ , attrcount++){
               Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
		         	attrCatBuffer.getRecord(attrCatRecord, attrcount);

			if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,
					   relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0)
			{
				const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER
										   ? "NUM" : "STR";
				printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
			}
			if (attrcount == attrCatHeader.numSlots-1) {
				attrcount = -1;
				attrCatBuffer = RecBuffer (attrCatHeader.rblock);
				attrCatBuffer.getHeader(&attrCatHeader);
			}
       }
          
        printf("\n");
    }

}
void updateattribute(const char *relname , const char * oldAttrname , const char *newAttrname){
          RecBuffer attrCatBuffer(ATTRCAT_BLOCK);
          HeadInfo attrCatHeader;
          attrCatBuffer.getHeader(&attrCatHeader);

          for(int i = 0 ; i<attrCatHeader.numEntries ; i++){
                 Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
                 attrCatBuffer.getRecord(attrCatRecord , i);
               if(strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal , relname) == 0 &&
                    strcmp(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal , oldAttrname)==0){

                       strcpy(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal , newAttrname);
                        attrCatBuffer.setRecord(attrCatRecord , i);
                        std::cout<< "Update the Attribute!\n\n";
                        break;
              }
              if(i == attrCatHeader.numSlots -1){
                     i =-1 ;
                    attrCatBuffer = RecBuffer (attrCatHeader.rblock);
                    attrCatBuffer.getHeader(&attrCatHeader);
              }
          }
}
int main(int argc, char *argv[]) {
  /* Initialize the Run Copy of Disk */
  Disk disk_run;
  StaticBuffer buffer ;
  OpenRelTable cache ;

  for(int i = 0 ; i<=2 ; i++){
        RelCatEntry relCatBuffer ;
        RelCacheTable::getRelCatEntry(i , &relCatBuffer);

        printf ("Relation: %s\n", relCatBuffer.relName);
		for (int attrIndex = 0; attrIndex < relCatBuffer.numAttrs; attrIndex++) {
			AttrCatEntry attrCatBuffer;
			AttrCacheTable::getAttrCatEntry(i, attrIndex, &attrCatBuffer);
			const char *attrType = attrCatBuffer.attrType == NUMBER ? "NUM" : "STR";
			printf ("    %s: %s\n", attrCatBuffer.attrName, attrType);
		}
		printf("\n");
  }
  return 0;
}
