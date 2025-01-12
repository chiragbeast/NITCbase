#include "BlockBuffer.h"

#include <cstdlib>
#include <cstring>

BlockBuffer::BlockBuffer(int blockNum) {
    this->blockNum = blockNum;
}

RecBuffer::RecBuffer(int blockNum) : BlockBuffer(blockNum) {}

int BlockBuffer::getHeader(struct HeadInfo *head) {
    unsigned char buffer[BLOCK_SIZE];

    // Read the block into the buffer
    if (Disk::readBlock(buffer, this->blockNum) != SUCCESS) {
        return FAILURE;
    }

    // Populate the header fields
    memcpy(&head->numSlots, buffer + 24, sizeof(int));
    memcpy(&head->numEntries, buffer + 16, sizeof(int));
    memcpy(&head->numAttrs, buffer + 20, sizeof(int));
    memcpy(&head->rblock, buffer + 12, sizeof(int));
    memcpy(&head->lblock, buffer + 8, sizeof(int));

    return SUCCESS;
}

int RecBuffer::getRecord(union Attribute *rec, int slotNum) {
    struct HeadInfo head;

    // Get the header information
    if (this->getHeader(&head) != SUCCESS) {
        return FAILURE;
    }

    unsigned char buffer[BLOCK_SIZE];

    // Read the block into the buffer
    if (Disk::readBlock(buffer, this->blockNum) != SUCCESS) {
        return FAILURE;
    }

    int recordSize = head.numAttrs * ATTR_SIZE;
    int slotMapSize = head.numSlots;
    int recordOffset = HEADER_SIZE + slotMapSize + (recordSize * slotNum);

    // Load the record into the rec structure
    memcpy(rec, buffer + recordOffset, recordSize);

    return SUCCESS;
}

int RecBuffer::setRecord(union Attribute *rec, int slotNum) {
    struct HeadInfo head;
    BlockBuffer::getHeader(&head);

    unsigned char buffer[BLOCK_SIZE];

    // Read the block into the buffer
    if (Disk::readBlock(buffer, this->blockNum) != SUCCESS) {
        return FAILURE;
    }

    int recordSize = head.numAttrs * ATTR_SIZE;
    int slotMapSize = head.numSlots;
    int recordOffset = HEADER_SIZE + slotMapSize + (recordSize * slotNum);

    // Load the record into the rec structure
    memcpy( buffer + recordOffset, rec, recordSize);

    Disk::writeBlock(buffer , this->blockNum);

    return SUCCESS;
}
int BlockBuffer::loadBlockAndGetBufferPtr(unsigned char **bufferptr){
            int bufferNum = StaticBuffer::getBufferNum(this->blockNum);

         if (bufferNum == E_BLOCKNOTINBUFFER){
        bufferNum = StaticBuffer::getFreeBuffer(this->blockNum);

        if (bufferNum == E_OUTOFBOUND)
        {
            return E_OUTOFBOUND;
        }

        Disk::readBlock(StaticBuffer::blocks[bufferNum], this->blockNum); 
    }
         *bufferptr = StaticBuffer::blocks[bufferNum];

         return SUCCESS ;
}
