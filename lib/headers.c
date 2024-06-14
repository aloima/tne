#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../tne.h"

void tne_add_header(struct TNEHeaders *headers, char *name, char *value, unsigned int name_len, unsigned int value_len) {
  ++headers->count;

  if (headers->data == NULL) {
    headers->data = malloc(sizeof(tneheader_t));
  } else {
    headers->data = realloc(headers->data, headers->count * sizeof(tneheader_t));
  }

  tneheader_t *header = &headers->data[headers->count - 1];

  header->name = malloc(name_len + 1);
  header->name_len = name_len;
  tne_strncpy(header->name, name, name_len);

  header->value = malloc(value_len + 1);
  header->value_len = value_len;
  tne_strncpy(header->value, value, value_len);
}

void tne_remove_header(struct TNEHeaders *headers, char *name) {
  for (unsigned int i = 0; i < headers->count; ++i) {
    tneheader_t *header = &headers->data[i];

    if (strcmp(header->name, name) == 0) {
      tneheader_t last_header = headers->data[headers->count - 1];

      header->name = realloc(header->name, last_header.name_len + 1);
      tne_strncpy(header->name, last_header.name, last_header.name_len);

      header->name_len = last_header.name_len;
      free(last_header.name);


      header->value = realloc(header->value, last_header.value_len + 1);
      tne_strncpy(header->value, last_header.value, last_header.value_len);

      header->value_len = last_header.value_len;
      free(last_header.value);

      --headers->count;
      headers->data = realloc(headers->data, headers->count * sizeof(tneheader_t));

      break;
    }
  }
}

tneheader_t *tne_get_header(struct TNEHeaders headers, char *name) {
  for (unsigned int i = 0; i < headers.count; ++i) {
    tneheader_t *header = &headers.data[i];
    unsigned int k = 0;
    while (name[k] != '\0' && header->name[k] != '\0' && tolower(name[k]) == tolower(header->name[k])) ++k;
    if (k == header->name_len) return header;
  }

  return NULL;
}

void tne_free_headers(struct TNEHeaders headers) {
  for (unsigned int i = 0; i < headers.count; ++i) {
    tneheader_t header = headers.data[i];
    free(header.name);
    free(header.value);
  }

  free(headers.data);
}
