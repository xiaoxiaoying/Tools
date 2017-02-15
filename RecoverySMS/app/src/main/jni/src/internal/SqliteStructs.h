#ifndef HUDUN_SQLITE_SQLITESTRUCTS_H
#define HUDUN_SQLITE_SQLITESTRUCTS_H
#include <cstdint>
namespace hudun
{
namespace sqlite
{
#pragma pack(push, 1)
typedef struct STRUCT_DATABASE_HEADER StructDatabaseHeader;
struct STRUCT_DATABASE_HEADER
{
    char	magicNumber[16];
    uint16_t pageSize;
    uint8_t fileFormatWriteVersion;
    uint8_t fileFormatReadVersion;
    uint8_t reservedSpacePerPage;
    uint8_t maxEmbeddedPayloadFraction;
    uint8_t minEmbeddedPayloadFraction;
    uint8_t leafPayloadFraction;
    uint32_t fileChangeCounter;
    uint32_t pages;
    uint32_t firstFreelistPageNumber;
    uint32_t freelistPages;
    uint32_t schemaCookie;
    uint32_t schemaFormatNumber;
    int32_t defaultPageCacheSize;
    uint32_t largestRootBTreePageNumber;
    uint32_t textEncoding;
    uint32_t userVersion;
    uint32_t incrementalVacuumMode;
    uint32_t applicationId;
    char unused[20];
    uint32_t versionValidFor;
    uint32_t sqliteVersionNumber;
};

typedef struct STRUCT_PAGE_BTREE_HEADER StructPageBtreeHeader;
struct STRUCT_PAGE_BTREE_HEADER
{
    char pageFlag;
    uint16_t firstBlockOffset;
    uint16_t cells;
    uint16_t contentOffset;
    uint8_t fragmentedFreeBytes;
    uint32_t rightmostPageNumber;
};
#pragma pack(pop)
}
}
#endif // HUDUN_SQLITE_SQLITESTRUCTS_H
