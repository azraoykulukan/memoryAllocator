/*22120205059-Azra Öykü Ulukan*/

#include "mymalloc.h"
#include <stdio.h>
#include <unistd.h> // for sbrk

// Bellek bloğu araması
Block *find_free_block(size_t size);

// Boyutu 16 bayt sınırına hizalama
size_t align_size(size_t size) {
    return (size + 15) & ~15;
}

// Yeterli alanın olup olmadığını kontrol et, yoksa sbrk kullanarak genişlet
void *mymalloc(size_t size) {
    size = align_size(size);

    if (heap_start == NULL) {
        heap_start = sbrk(0);
        if (sbrk(HEAP_SIZE) == (void *)-1) {
            return NULL;
        }
        heap_end = heap_start + HEAP_SIZE;
        Block *new_block = (Block *)heap_start;
        new_block->info.size = HEAP_SIZE / 16; // 16 bayt bloklar
        new_block->info.isfree = 1;
        new_block->next = NULL;
        new_block->prev = NULL;
        free_list = new_block;
    }

    Block *fitting_block = (Block *) find_free_block(size);

    if (fitting_block == NULL) {
        return NULL;
    }

    if (fitting_block->info.size >= size / 16 + 1 + sizeof(Block)) {
        Block *split_result = split_block(fitting_block, size / 16 + 1);
        return split_result->data;
    } else {
        fitting_block->info.isfree = 0;
        return fitting_block->data;
    }
}

// Serbest bir blok bul
Block *find_free_block(size_t size) {
    Block *curr = free_list;
    while (curr != NULL) {
        if (curr->info.size >= size / 16 && curr->info.isfree) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// Belleği serbest bırak
void myfree(void *p) {
    if (p == NULL) {
        return;
    }
    Block *to_free = (Block *)((char *)p - offsetof(Block, data));
    to_free->info.isfree = 1;
    left_coalesce(to_free);
    right_coalesce(to_free);
}

// Bloğu bölmek
Block *split_block(Block *b, size_t size) {
    size_t remaining_size = b->info.size - size;
    Block *split = (Block *)((char *)b + sizeof(Block) + size * 16);
    split->info.size = remaining_size;
    split->info.isfree = 1;
    split->next = b->next;
    split->prev = b;
    b->info.size = size;
    b->info.isfree = 0;
    b->next = split;
    if (split->next != NULL) {
        split->next->prev = split;
    }
    return b;
}

// Adrese göre önceki bloğu döndür
Block *prev_block_in_addr(Block *b) {
    if (b == heap_start) {
        return NULL;
    }
    return (Block *)((char *)b - sizeof(Block) - b->prev->info.size * 16);
}

// Adrese göre sonraki bloğu döndür
Block *next_block_in_addr(Block *b) {
    if (b == heap_end) {
        return NULL;
    }
    return (Block *)((char *)b + sizeof(Block) + b->info.size * 16);
}

// Sol tarafta birleştir
Block *left_coalesce(Block *b) {
    if (b == NULL || b == heap_start) {
        return b;
    }

    Block *prev = prev_block_in_addr(b);

    if (prev != NULL && prev->info.isfree) {
        prev->info.size += sizeof(Block) / 16 + b->info.size;
        prev->next = b->next;
        if (b->next != NULL) {
            b->next->prev = prev;
        }
        return prev;
    }

    return b;
}

// Sağ tarafta birleştir
Block *right_coalesce(Block *b) {
    if (b == NULL || b == heap_end) {
        return b;
    }

    Block *next = next_block_in_addr(b);

    if (next != NULL && next->info.isfree) {
        b->info.size += sizeof(Block) / 16 + next->info.size;
        b->next = next->next;
        if (next->next != NULL) {
            next->next->prev = b;
        }
    }

    return b;
}

// Boyutu 16 bayt blokların sayısına dönüştür
uint64_t numberof16blocks(size_t size_inbytes) {
    size_t aligned_size = (size_inbytes + 15) & ~15; // 16 bayt sınıra hizala
    uint64_t blocks = aligned_size / 16; // 16 bayt blokların sayısını hesapla
    if (aligned_size % 16 != 0) {
        blocks++; // Kalan baytları ek bir blok olarak dahil et
    }
    return blocks;
}

// Belleği yazdır
void printheap() {
    printf("Blocks\n");
    Block *curr = heap_start;
    while (curr < heap_end) {
        printf("Size: %" PRIu64 "\n", curr->info.size);
        printf("Free: %d\n", curr->info.isfree);
        printf("--------\n");
        curr = (Block *)((char *)curr + sizeof(Block) + curr->info.size * 16);
    }
}

// Liste türünü al
ListType getlisttype() {
    return listtype;
}

// Liste türünü ayarla
int setlisttype(ListType new_listtype) {
    listtype = new_listtype;
    return 0;
}

// Stratejiyi al
Strategy getstrategy() {
    return strategy;
}

// Stratejiyi ayarla
int setstrategy(Strategy new_strategy) {
    if (new_strategy >= BEST_FIT && new_strategy <= WORST_FIT) {
        strategy = new_strategy;
        return 0;
    } else {
        return -1;
    }
}

// Ana fonksiyon
int main() {
    // Örnek kullanım
    printf("Başlangıç Heap'i:\n");
    printheap();

    // Bellek tahsisleri
    void *ptr1 = mymalloc(32);
    void *ptr2 = mymalloc(64);

    printf("Tahsislerden sonra Heap:\n");
    printheap();

    // Bellek bırakma
    myfree(ptr1);

    printf("ptr1 serbest bırakıldıktan sonra Heap:\n");
    printheap();

    return 0;
}
