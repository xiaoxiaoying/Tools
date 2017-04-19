//
// Created by Administrator on 2016/11/11/011.
//

#include <stdio.h>
#include "jpgcom.h"
#include <android/log.h>
#include <string.h>
#include <errno.h>

#ifdef HAVE_LOCALE_H
#include <locale.h>		/* Bill Allombert: use locale for isprint */
#endif

#include <ctype.h>		/* to declare isupper(), tolower() */
#include <stdbool.h>


#define TAG "libso"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

BYTE *bytes;
static long long index = 0;
long long byte_length = 0;

/* Return next input byte, or EOF if no more */
static int NEXTBYTE() {
    if (index >= byte_length) {
        LOGE("byte_length == %lld;index === %lld", byte_length, index);
        return -1;
    }
    int i = bytes[index];
    index++;
    return i;
}

/*
 * Read the initial marker, which should be SOI.
 * For a JFIF file, the first two bytes of the file should be literally
 * 0xFF M_SOI.  To be more general, we could use next_marker, but if the
 * input file weren't actually JPEG at all, next_marker might read the whole
 * file and then return a misleading error message...
 */

static int
first_marker(void) {
    int c1, c2;
    index = 0;
    c1 = NEXTBYTE();
    c2 = NEXTBYTE();
    if (c1 != 0xFF || c2 != M_SOI) {
        return -1;
    }
    return c2;
}

/* Read one byte, testing for EOF */
static int
read_1_byte(void) {
    int c;
    c = NEXTBYTE();
    if (c == EOF)
        LOGE("Premature EOF in JPEG file");
    return c;
}

/*
 * Find the next JPEG marker and return its marker code.
 * We expect at least one FF byte, possibly more if the compressor used FFs
 * to pad the file.
 * There could also be non-FF garbage between markers.  The treatment of such
 * garbage is unspecified; we choose to skip over it but emit a warning msg.
 * NB: this routine must not be used after seeing SOS marker, since it will
 * not deal correctly with FF/00 sequences in the compressed image data...
 */

static int
next_marker(void) {
    int c;
    int discarded_bytes = 0;

    /* Find 0xFF byte; count and skip any non-FFs. */
    c = read_1_byte();
    while (c != 0xFF && c > 0) {
        discarded_bytes++;
        c = read_1_byte();
    }
    /* Get marker code byte, swallowing any duplicate FF bytes.  Extra FFs
     * are legal as pad bytes, so don't count them in discarded_bytes.
     */
    do {
        c = read_1_byte();
    } while (c == 0xFF);

    if (discarded_bytes != 0) {
        fprintf(stderr, "Warning: garbage data found in JPEG file\n");
    }

    return c;
}

/* Read 2 bytes, convert to unsigned int */
/* All 2-byte quantities in JPEG markers are MSB first */
static unsigned int
read_2_bytes(void) {
    int c1, c2;

    c1 = NEXTBYTE();

    if (c1 < 0) {

        return (unsigned int) -1;
    }

    if (c1 == EOF)
        LOGE("Premature EOF in JPEG file");
    c2 = NEXTBYTE();
    if (c2 == EOF)
        LOGE("Premature EOF in JPEG file");
    return (((unsigned int) c1) << 8) + ((unsigned int) c2);
}

/*
 * Process a SOFn marker.
 * This code is only needed if you want to know the image dimensions...
 */

static int process_SOFn(int marker) {
    unsigned int length;
    unsigned int image_height, image_width;
    int data_precision, num_components;
    const char *process;
    int ci;

    switch (marker) {
        case M_SOF0:
            process = "Baseline";
            break;
        case M_SOF1:
            process = "Extended sequential";
            break;
        case M_SOF2:
            process = "Progressive";
            break;
        case M_SOF3:
            process = "Lossless";
            break;
        case M_SOF5:
            process = "Differential sequential";
            break;
        case M_SOF6:
            process = "Differential progressive";
            break;
        case M_SOF7:
            process = "Differential lossless";
            break;
        case M_SOF9:
            process = "Extended sequential, arithmetic coding";
            break;
        case M_SOF10:
            process = "Progressive, arithmetic coding";
            break;
        case M_SOF11:
            process = "Lossless, arithmetic coding";
            break;
        case M_SOF13:
            process = "Differential sequential, arithmetic coding";
            break;
        case M_SOF14:
            process = "Differential progressive, arithmetic coding";
            break;
        case M_SOF15:
            process = "Differential lossless, arithmetic coding";
            break;
        default:
            process = "Unknown";
            break;
    }
    LOGD("JPEG process: %s\n", process);


    length = read_2_bytes();    /* usual parameter length count */

    data_precision = read_1_byte();
    image_height = read_2_bytes();
    image_width = read_2_bytes();
    num_components = read_1_byte();

    LOGD("JPEG image is %uw * %uh, %d color components, %d bits per sample\n",
         image_width, image_height, num_components, data_precision);
    LOGD("image length === %d", length);


    if (length != (unsigned int) (8 + num_components * 3)) {

        LOGE("Bogus SOF marker ");
        return -1;
    }

    for (ci = 0; ci < num_components; ci++) {
        (void) read_1_byte();    /* Component ID code */
        (void) read_1_byte();    /* H, V sampling factors */
        (void) read_1_byte();    /* Quantization table number */
    }
    return 0;
}


/*
 * Process a COM marker.
 * We want to print out the marker contents as legible text;i
 * we must guard against non-text junk and varying newline representations.
 */

static int
process_COM(int raw) {
    unsigned int length;
    int ch;
    int lastch = 0;

    /* Bill Allombert: set locale properly for isprint */
#ifdef HAVE_LOCALE_H
    setlocale(LC_CTYPE, "");
#endif

    /* Get the marker parameter length count */
    length = read_2_bytes();
    /* Length includes itself, so must be at least 2 */
    if (length < 2) {
        LOGE("Erroneous JPEG marker length");
        return -1;
    }
    length -= 2;
    LOGD("length ================= %d", length);
    while (length > 0) {
        ch = read_1_byte();
        if (ch < 0) {
            return -1;
        }

        if (raw) {
            putc(ch, stdout);
            /* Emit the character in a readable form.
             * Nonprintables are converted to \nnn form,
             * while \ is converted to \\.
             * Newlines in CR, CR/LF, or LF form will be printed as one newline.
             */
        } else if (ch == '\r') {
            LOGD("\n");
        } else if (ch == '\n') {
            if (lastch != '\r')
                LOGD("\n");
        } else if (ch == '\\') {
            LOGD("\\\\");
        } else if (isprint(ch)) {
            putc(ch, stdout);
        } else {
            LOGD("\\%03o", ch);
        }
        lastch = ch;
        length--;
    }
    LOGD("\n");

    /* Bill Allombert: revert to C locale */
#ifdef HAVE_LOCALE_H
    setlocale(LC_CTYPE, "C");
#endif

    return 0;
}


/*
 * Most types of marker are followed by a variable-length parameter segment.
 * This routine skips over the parameters for any marker we don't otherwise
 * want to process.
 * Note that we MUST skip the parameter segment explicitly in order not to
 * be fooled by 0xFF bytes that might appear within the parameter segment;
 * such bytes do NOT introduce new markers.
 */

static int
skip_variable(void)
/* Skip over an unknown or uninteresting variable-length marker */
{
    unsigned int length;
    /* Get the marker parameter length count */
    length = read_2_bytes();
    LOGD("skip_variable length === %d", length);
    /* Length includes itself, so must be at least 2 */
    if (length < 2) {
        LOGE("Erroneous JPEG marker length");
        return -1;
    }
    length -= 2;
    /* Skip over the remaining bytes */
    while (length > 0) {
        int read = read_1_byte();
        if (read < 0)
            return -1;
        length--;
    }

    return 0;
}

static long long process_SOS(void) {
    for (; ;) {
        int marker = next_marker();
        if (marker == M_EOI) {
            return index;
        } else if (marker == -1) {
            return -1;
        }
    }
}
/*
 * Parse the marker stream until SOS or EOI is seen;
 * display any COM markers.
 * While the companion program wrjpgcom will always insert COM markers before
 * SOFn, other implementations might not, so we scan to SOS before stopping.
 * If we were only interested in the image dimensions, we would stop at SOFn.
 * (Conversely, if we only cared about COM markers, there would be no need
 * for special code to handle SOFn; we could treat it like other markers.)
 */
/*
 * JPEG markers consist of one or more 0xFF bytes, followed by a marker
 * code byte (which is not an FF).  Here are the marker codes of interest
 * in this program.  (See jdmarker.c for a more complete list.)
 */


static long long
scan_JPEG_header(int verbose, int raw) {
    int marker;
    int com = 0;
    /* Expect SOI at start of file */
    if (first_marker() != M_SOI) {
        LOGE("不是jpg图片");
        return -1;
    }

    /* Scan miscellaneous markers until we reach SOS. */
    for (; ;) {
        marker = next_marker();
        switch (marker) {
            /* Note that marker codes 0xC4, 0xC8, 0xCC are not, and must not be,
             * treated as SOFn.  C4 in particular is actually DHT.
             */
            case M_SOF0:        /* Baseline */
            case M_SOF1:        /* Extended sequential, Huffman */
            case M_SOF2:        /* Progressive, Huffman */
            case M_SOF3:        /* Lossless, Huffman */
            case M_SOF5:        /* Differential sequential, Huffman */
            case M_SOF6:        /* Differential progressive, Huffman */
            case M_SOF7:        /* Differential lossless, Huffman */
            case M_SOF9:        /* Extended sequential, arithmetic */
            case M_SOF10:        /* Progressive, arithmetic */
            case M_SOF11:        /* Lossless, arithmetic */
            case M_SOF13:        /* Differential sequential, arithmetic */
            case M_SOF14:        /* Differential progressive, arithmetic */
            case M_SOF15:        /* Differential lossless, arithmetic */
                if (verbose) {
                    LOGD("sofn start ============");
                    int sofn = process_SOFn(marker);
                    if (sofn != 0)
                        return sofn;
                } else {
                    skip_variable();
                }
                break;

            case M_SOS:            /* stop before hitting compressed data */
//                return marker;
                return process_SOS();
            case M_EOI:            /* in case it's a tables-only JPEG stream */
//                return marker;
                return index;
            case M_COM:
                com = process_COM(raw);
                if (com != 0) {
                    return -1;
                }

                break;

            case M_APP12:
            case M_APP1:
            case M_APP0:
                /* Some digital camera makers put useful textual information into
                 * APP12 markers, so we print those out too when in -verbose mode.
                 */

                if (verbose) {
                    LOGD("APP12 contains:\nraw == %d", raw);
                    com = process_COM(raw);
                } else
                    com = skip_variable();

                if (com != 0) {
                    return -1;
                }
                break;
            case -1:
                return -1;

            default:            /* Anything else just gets skipped */

                LOGD(" ============  default ======================");
                com = skip_variable();        /* we assume it has a parameter count... */
                if (com != 0)
                    return -1;
                break;
        }
    } /* end loop */
}

long long parse_jpg(BYTE *byte, long long length) {
    LOGD("length == %lld", length);
    bytes = malloc(sizeof(BYTE) * length);
    byte_length = length;
    memcpy(bytes, byte, length);
    long long len = scan_JPEG_header(1, 1);
    free(bytes);
    return len;
}