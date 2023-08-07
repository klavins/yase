/**
 * A simple ring buffer class. This is thread-safe so long as only a
 * single producer and a single consumer are clients.
 * http://shanekirk.com/category/programming/cc/
 */

#include <iostream>
#include <memory>

template<typename DataType> class RingBuffer {
public:

    RingBuffer(int capacity) :
        m_capacity(capacity),
        m_freeItems(capacity),
        m_readIndex(0),
        m_writeIndex(0) {

        static_assert(ATOMIC_INT_LOCK_FREE == 2);
        m_spBuffer = std::make_unique<DataType[]>(m_capacity);
        memset(m_spBuffer.get(), 0, sizeof(DataType) * m_capacity);

    }

    int getReadableItemCount() const {
        return m_capacity - m_freeItems.load();
    }

    int getWriteableItemCount() const { 
        return m_freeItems.load();      
    }       

    int read(DataType * pData, int max) {

        int readableItemCount = getReadableItemCount();
        if (readableItemCount == 0)
            return 0;

        int totalItemsToRead = std::min(readableItemCount, max);
        int itemsLeftToRead = totalItemsToRead;

        if (m_readIndex + itemsLeftToRead >= m_capacity)  {
            std::cout << "read wrap " << readableItemCount << ", " << m_readIndex << ", " << itemsLeftToRead << std::endl;
            int itemsAvailableUntilEnd = m_capacity - m_readIndex;
            memcpy(pData, m_spBuffer.get() + m_readIndex, itemsAvailableUntilEnd * sizeof(DataType));
            m_readIndex = 0;
            itemsLeftToRead -= itemsAvailableUntilEnd;
            pData += itemsAvailableUntilEnd;
        }

        if (itemsLeftToRead > 0) {
            memcpy(pData, m_spBuffer.get() + m_readIndex, itemsLeftToRead * sizeof(DataType));
            m_readIndex += itemsLeftToRead;
            itemsLeftToRead = 0;
        }

        m_freeItems += totalItemsToRead; // this is atomic
        return totalItemsToRead;
    }

    int write(DataType * pData, int numItems) {
        
        int writeableItemCount = getWriteableItemCount();
        if (writeableItemCount == 0)
            return 0;

        int totalItemsToWrite = std::min(writeableItemCount, numItems);
        int itemsLeftToWrite = totalItemsToWrite;

        if ((m_writeIndex + itemsLeftToWrite) >= m_capacity) {
            std::cout << "write wrap" << std::endl;
            int itemsAvailableUntilEnd = m_capacity - m_writeIndex;
            memcpy(m_spBuffer.get() + m_writeIndex, pData, itemsAvailableUntilEnd * sizeof(DataType));
            m_writeIndex = 0;
            itemsLeftToWrite -= itemsAvailableUntilEnd;
            pData += itemsAvailableUntilEnd;
        }

        if (itemsLeftToWrite > 0) {
            memcpy(m_spBuffer.get() + m_writeIndex, pData, itemsLeftToWrite * sizeof(DataType));
            m_writeIndex += itemsLeftToWrite;
            itemsLeftToWrite = 0;
        }

        m_freeItems -= totalItemsToWrite; // this is atomic
        return totalItemsToWrite;

    }
private:

    std::unique_ptr<DataType[]> m_spBuffer; // The data.
    int m_writeIndex; // Where the producer is writing to next.
    int m_readIndex;  // Where the consumer is reading from next.
 
    const int m_capacity;         // Total number of frames managed by the ring buffer.
    std::atomic<int> m_freeItems; // Number of frames that are available to be written into.

};