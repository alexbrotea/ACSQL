#include "task3.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static inline size_t pbox_func(size_t i, size_t n) {
    return (i * (n - 1) + 2) % n;
}

static void p_box(uint8_t *data, size_t len) {
    uint8_t *temp = (uint8_t *)malloc(len);
    if (!temp) return;


    memset(temp, 0, len);
    for (size_t i = 0; i < len; i++) {
        size_t new_pos = pbox_func(i, len);
        temp[new_pos] = data[i];
    }
    memcpy(data, temp, len);
    free(temp);
}

static void s_box(uint8_t *data, size_t data_len, const uint8_t *key, size_t key_len) {
    for (size_t i = 0; i < data_len; i++) {
        size_t k = i % key_len;
        data[i] ^= key[k];
    }
}

static void xor_block(uint8_t *out, const uint8_t *in1, const uint8_t *in2, size_t len) {
    for (size_t i = 0; i < len; i++) {
        out[i] = in1[i] ^ in2[i];
    }
}

static void extinde_IV(const uint8_t *iv, size_t iv_len, uint8_t *out, size_t block_size) {
    for (size_t i = 0; i < block_size; i++) {
        out[i] = iv[i % iv_len];
    }
}

void cripteaza_studenti(secretariat *secretariat,
                        void *key, size_t key_len,
                        void *iv,  size_t iv_len,
                        char *cale_output) {
    if (!secretariat || !secretariat->studenti || secretariat->nr_studenti <= 0) {
        return;
    }


    size_t total_students = (size_t)secretariat->nr_studenti;
    size_t total_len = total_students * sizeof(student);

    uint8_t *buffer = (uint8_t *)calloc(total_len, 1);
    if (!buffer) {
        return;
    }

    memcpy(buffer, (uint8_t *)secretariat->studenti, total_len);

    size_t block_size = (total_len + 4 - 1) / 4;
    size_t final_len  = block_size * 4;

    uint8_t *all_data = (uint8_t *)calloc(final_len, 1);
    if (!all_data) {
        free(buffer);
        return;
    }

    memcpy(all_data, buffer, total_len);
    free(buffer);
    buffer = NULL;

    uint8_t *iv_extended = (uint8_t *)malloc(block_size);
    if (!iv_extended) {
        free(all_data);
        return;
    }
    extinde_IV((uint8_t*)iv, iv_len, iv_extended, block_size);

    uint8_t *temp_block = (uint8_t *)malloc(block_size);
    if (!temp_block) {
        free(iv_extended);
        free(all_data);
        return;
    }

    xor_block(temp_block, all_data, iv_extended, block_size);
    s_box(temp_block, block_size, (uint8_t *)key, key_len);
    p_box(temp_block, block_size);
    memcpy(all_data, temp_block, block_size);

    for (int i = 1; i < 4; i++) {
        uint8_t *cur_block  = all_data + i * block_size;
        uint8_t *prev_block = all_data + (i - 1) * block_size;

        xor_block(temp_block, cur_block, prev_block, block_size);
        s_box(temp_block, block_size, (uint8_t *)key, key_len);
        p_box(temp_block, block_size);
        memcpy(cur_block, temp_block, block_size);
    }

    free(temp_block);
    free(iv_extended);

    FILE *outf = fopen(cale_output, "wb");
    if (!outf) {
        free(all_data);
        return;
    }
    fwrite(all_data, 1, final_len, outf);
    fclose(outf);

    free(all_data);
}
