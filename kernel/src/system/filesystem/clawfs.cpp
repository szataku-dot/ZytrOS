#include "clawfs.h"
#include "system/drivers/video/driver.h"
#include "libs/libc/libc.h"
#include "system/drivers/ata/driver.h"
#include "applications/shell/commands.h"

void clawfs_format()
{
    // 1. Formatowanie nagłówka (Sektor 100)
    uint8_t sector_buffer[512] = {0};
    CLAWFSHeader* header = (CLAWFSHeader*)sector_buffer;

    header->signature[0] = 'C';
    header->signature[1] = 'L';
    header->signature[2] = 'A';
    header->signature[3] = 'W';
    header->signature[4] = 'F';
    header->signature[5] = 'S';
    header->version = CLAWFS_VERSION;
    header->entryCount = 4; // Zaczynamy od 4 domyślnych folderów

    disk_write_sector(CLAWFS_START_LBA, sector_buffer);

    // 2. Formatowanie pierwszych wpisów (Sektor 101)
    uint8_t entries_buffer[512] = {0};
    CLAWFSEntry* entries = (CLAWFSEntry*)entries_buffer;

    strcpy(entries[0].name, "bin");
    entries[0].type = CLAWFS_DIRECTORY;

    strcpy(entries[1].name, "dev");
    entries[1].type = CLAWFS_DIRECTORY;

    strcpy(entries[2].name, "etc");
    entries[2].type = CLAWFS_DIRECTORY;

    strcpy(entries[3].name, "home");
    entries[3].type = CLAWFS_DIRECTORY;

    disk_write_sector(CLAWFS_START_LBA + 1, entries_buffer);
}

void clawfs_create_file(const char* name)
{
    // 1. Odczytaj nagłówek, aby wiedzieć, ile mamy już wpisów
    uint8_t header_buffer[512];
    disk_read_sector(CLAWFS_START_LBA, header_buffer);
    CLAWFSHeader* header = (CLAWFSHeader*)header_buffer;

    // 2. SPRAWDZENIE DUPLIKATÓW: Przeszukaj istniejące wpisy na dysku
    uint8_t search_buffer[512];
    uint32_t current_loaded_sector = 0;
    CLAWFSEntry* search_entries = nullptr;

    for (uint32_t i = 0; i < header->entryCount; i++)
    {
        uint32_t required_sector_offset = 1 + (i / 16);
        uint32_t index_in_sector = i % 16;

        // Wczytujemy sektor tylko jeśli jeszcze nie mamy go w pamięci RAM
        if (current_loaded_sector != required_sector_offset) {
            disk_read_sector(CLAWFS_START_LBA + required_sector_offset, search_buffer);
            current_loaded_sector = required_sector_offset;
            search_entries = (CLAWFSEntry*)search_buffer;
        }

        // strcmp zwraca 0, jeśli nazwy są dokładnie takie same
        if (strcmp(search_entries[index_in_sector].name, name) == 0) {
            print_error("File '");
            print(name);
            print("' aleredy exists!\n");
            return; // Przerywamy funkcję, nie tworzymy duplikatu
        }
    }

    // 3. Zabezpieczenie przed przepełnieniem tablicy (max 128 wpisów)
    if (header->entryCount >= 128) {
        print_error("No space anvible\n");
        return;
    }

    // 4. Oblicz, gdzie zapisać nowy plik (skoro nazwa jest unikalna)
    uint32_t target_index = header->entryCount;
    uint32_t sector_offset = 1 + (target_index / 16);
    uint32_t index_in_sector = target_index % 16;

    // Odczytaj docelowy sektor przed modyfikacją
    uint8_t entries_buffer[512];
    disk_read_sector(CLAWFS_START_LBA + sector_offset, entries_buffer);
    CLAWFSEntry* entries = (CLAWFSEntry*)entries_buffer;

    // Uzupełnij dane nowego wpisu
    strcpy(entries[index_in_sector].name, name);
    entries[index_in_sector].type = CLAWFS_FILE;

    // Zapisz zaktualizowany sektor z wpisami z powrotem na dysk
    disk_write_sector(CLAWFS_START_LBA + sector_offset, entries_buffer);

    // Zwiększ licznik plików w nagłówku i zapisz go
    header->entryCount++;
    disk_write_sector(CLAWFS_START_LBA, header_buffer);

    print_info("File created: ");
    print(name);
    print("\n");
}

void clawfs_dir()
{
    uint8_t header_buffer[512];
    disk_read_sector(CLAWFS_START_LBA, header_buffer);
    CLAWFSHeader* header = (CLAWFSHeader*)header_buffer;

    uint8_t entries_buffer[512];
    uint32_t current_loaded_sector = 0;
    CLAWFSEntry* entries = nullptr;

    for(uint32_t i = 0; i < header->entryCount; i++)
    {
        // Obliczamy dynamicznie, w którym sektorze leży dany wpis
        uint32_t required_sector_offset = 1 + (i / 16);
        uint32_t index_in_sector = i % 16;

        // Czytamy nowy sektor z dysku tylko wtedy, gdy jeszcze go nie mamy w pamięci RAM
        if (current_loaded_sector != required_sector_offset) {
            disk_read_sector(CLAWFS_START_LBA + required_sector_offset, entries_buffer);
            current_loaded_sector = required_sector_offset;
            entries = (CLAWFSEntry*)entries_buffer;
        }

        if(entries[index_in_sector].type == CLAWFS_DIRECTORY)
            print("<DIR> ");
        else
            print("      ");

        print(entries[index_in_sector].name);
        print("\n");
    }
}

bool clawfs_exists()
{
    uint8_t buffer[512];
    disk_read_sector(CLAWFS_START_LBA, buffer);
    CLAWFSHeader* header = (CLAWFSHeader*)buffer;

    bool isValid = (
        header->signature[0] == 'C' &&
        header->signature[1] == 'L' &&
        header->signature[2] == 'A' &&
        header->signature[3] == 'W' &&
        header->signature[4] == 'F' &&
        header->signature[5] == 'S' &&
        header->version == CLAWFS_VERSION
    );

    if (isValid) {
        print("Partition Mounted\n");
    }
    
    return isValid;
}