#ifndef LAYERS_H
#define LAYERS_H

#define OSI_BUFFER_SIZE 8192

int encapsulate(const char *message, char *output, unsigned long output_size);
int decapsulate(const char *packet, char *output, unsigned long output_size);

#endif
