#include <stdio.h>
#include <string.h>
#include "quirc_internal.h"
#include "dbgutil.h"

// modify from quirc/tests/inspect.c
// remove SDL support, and add image.raw output

struct dump_result {
    unsigned long width;
    unsigned long height;
    unsigned long length;
};

static int dump_imgdata(struct quirc *q, const char *filename, struct dump_result *dr)
{
    FILE *outfile = NULL;
    if ((outfile = fopen(filename, "wb")) == NULL) {
        return -1;
    }

    unsigned long length = q->w * q->h;
    fwrite(q->image, length, 1, outfile);
    fclose(outfile);

    dr->width = q->w;
    dr->height = q->h;
    dr->length = length;
    return 0;
}

static void dump_info(struct quirc *q)
{
    int count = quirc_count(q);
    int i;

    printf("%d QR-codes found:\n\n", count);
    for (i = 0; i < count; i++) {
        struct quirc_code code;
        struct quirc_data data;
        quirc_decode_error_t err;

        quirc_extract(q, i, &code);
        err = quirc_decode(&code, &data);

        dump_cells(&code);
        printf("\n");

        if (err) {
            printf("  Decoding FAILED: %s\n", quirc_strerror(err));
        } else {
            printf("  Decoding successful:\n");
            dump_data(&data);
        }

        printf("\n");
    }
}

int main(int argc, char **argv)
{
    struct quirc *q;

    printf("Quirc version: %s\n", quirc_version());
    printf("\n");

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <testfile.jpg|testfile.png> [image.raw]\n", argv[0]);
        return -1;
    }

    q = quirc_new();
    if (!q) {
        perror("can't create quirc object");
        return -1;
    }

    int status = -1;
    if (check_if_png(argv[1])) {
        status = load_png(q, argv[1]);
    } else {
        status = load_jpeg(q, argv[1]);
    }
    if (status < 0) {
        quirc_destroy(q);
        return -1;
    }

    struct dump_result dr;
    int di_status = 0;
    if (argc == 3) {
        di_status = dump_imgdata(q, argv[2], &dr);
    }

    quirc_end(q);
    dump_info(q);

    if (argc == 3) {
        if (di_status < 0) {
            printf("Fail to write image raw file:\n");
            printf("    file: %s\n", argv[2]);
        } else {
            printf("Success to write image raw file:\n");
            printf("    file: %s\n", argv[2]);
            printf("    width: %ld pixels\n", dr.width);
            printf("    height: %ld pixels\n", dr.height);
            printf("    length: %ld bytes\n", dr.length);
        }
    }

    quirc_destroy(q);
    return 0;
}
