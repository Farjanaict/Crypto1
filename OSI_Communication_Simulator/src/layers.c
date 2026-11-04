#include <stdio.h>
#include <string.h>
#include "layers.h"

static const char *layer_names[7] = {
    "Application", "Presentation", "Session", "Transport",
    "Network", "DataLink", "Physical"
};

static const int layer_numbers[7] = {7, 6, 5, 4, 3, 2, 1};

static int wrap_one(const char *input, char *output, unsigned long size,
                    int layer_number, const char *layer_name) {
    int written = snprintf(output, size, "L%d[%s]", layer_number, input);
    if (written < 0 || (unsigned long)written >= size) return -1;
    printf("[ENCAP] Layer %d - %-12s : header added\n",
           layer_number, layer_name);
    return 0;
}

static int unwrap_one(const char *input, char *output, unsigned long size,
                      int layer_number, const char *layer_name) {
    char prefix[16];
    size_t prefix_len;
    size_t input_len;

    snprintf(prefix, sizeof(prefix), "L%d[", layer_number);
    prefix_len = strlen(prefix);
    input_len = strlen(input);

    if (input_len < prefix_len + 1 ||
        strncmp(input, prefix, prefix_len) != 0 ||
        input[input_len - 1] != ']') {
        fprintf(stderr, "[DECAP] Invalid data at Layer %d (%s)\n",
                layer_number, layer_name);
        return -1;
    }

    if (input_len - prefix_len - 1 >= size) return -1;
    memcpy(output, input + prefix_len, input_len - prefix_len - 1);
    output[input_len - prefix_len - 1] = '\0';

    printf("[DECAP] Layer %d - %-12s : header removed\n",
           layer_number, layer_name);
    return 0;
}

int encapsulate(const char *message, char *output, unsigned long output_size) {
    char current[OSI_BUFFER_SIZE];
    char next[OSI_BUFFER_SIZE];
    int i;

    if (strlen(message) >= sizeof(current)) return -1;
    strcpy(current, message);

    printf("\n========== SENDER: ENCAPSULATION ==========\n");
    for (i = 0; i < 7; i++) {
        if (wrap_one(current, next, sizeof(next),
                     layer_numbers[i], layer_names[i]) != 0) return -1;
        strcpy(current, next);
    }

    if (strlen(current) + 1 > output_size) return -1;
    strcpy(output, current);
    printf("Encapsulation completed. Packet is ready.\n");
    return 0;
}

int decapsulate(const char *packet, char *output, unsigned long output_size) {
    char current[OSI_BUFFER_SIZE];
    char next[OSI_BUFFER_SIZE];
    int i;

    if (strlen(packet) >= sizeof(current)) return -1;
    strcpy(current, packet);

    printf("\n========== RECEIVER: DECAPSULATION ==========\n");
    for (i = 6; i >= 0; i--) {
        if (unwrap_one(current, next, sizeof(next),
                       layer_numbers[i], layer_names[i]) != 0) return -1;
        strcpy(current, next);
    }

    if (strlen(current) + 1 > output_size) return -1;
    strcpy(output, current);
    printf("Decapsulation completed. Original message recovered.\n");
    return 0;
}
