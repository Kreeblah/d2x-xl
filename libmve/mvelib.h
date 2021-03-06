#ifndef INCLUDED_MVELIB_H
#define INCLUDED_MVELIB_H

#include <stdio.h>
#include <stdlib.h>

#include "decoders.h"
#include "libmve.h"

extern mve_cb_Read mve_read;
extern mve_cb_Alloc mve_alloc;
extern mve_cb_Free mve_free;
extern mve_cb_ShowFrame mve_showframe;
extern mve_cb_SetPalette mve_setpalette;

/*
 * structure for maintaining info on a MVEFILE stream
 */
typedef struct MVEFILE
{
    void    *stream;
    uint8_t	*cur_chunk;
    int32_t  buf_size;
    int32_t  cur_fill;
    int32_t  next_segment;
} MVEFILE;

/*
 * open a .MVE file
 */
MVEFILE *mvefile_open(void *stream);

/*
 * close a .MVE file
 */
void mvefile_close(MVEFILE *movie);

/*
 * get size of next tSegment in chunk (-1 if no more segments in chunk)
 */
int32_t mvefile_get_next_segment_size(MVEFILE *movie);

/*
 * get nType of next tSegment in chunk (0xff if no more segments in chunk)
 */
uint8_t mvefile_get_next_segmentMajor(MVEFILE *movie);

/*
 * get subtype (version) of next tSegment in chunk (0xff if no more segments in
 * chunk)
 */
uint8_t mvefile_get_next_segmentMinor(MVEFILE *movie);

/*
 * see next tSegment (return NULL if no next tSegment)
 */
uint8_t *mvefile_get_next_segment(MVEFILE *movie);

/*
 * advance to next tSegment
 */
void mvefile_advance_segment(MVEFILE *movie);

/*
 * fetch the next chunk (return 0 if at end of stream)
 */
int32_t mvefile_fetch_next_chunk(MVEFILE *movie);

/*
 * callback for tSegment nType
 */
typedef int32_t (*MVESEGMENTHANDLER)(uint8_t major, uint8_t minor, uint8_t *data, int32_t len, void *context);

/*
 * structure for maintaining an MVE stream
 */
typedef struct MVESTREAM
{
    MVEFILE             *movie;
    void                *context;
    MVESEGMENTHANDLER   handlers[32];
	 int32_t					bLittleEndian;
} MVESTREAM;

/*
 * open an MVE stream
 */
MVESTREAM *mve_open(void *stream, int32_t bLittleEndian);

/*
 * close an MVE stream
 */
void mve_close(MVESTREAM *movie);

/*
 * reset an MVE stream
 */
void mve_reset(MVESTREAM *movie);

/*
 * set tSegment nType handler
 */
void mve_set_handler(MVESTREAM *movie, uint8_t major, MVESEGMENTHANDLER handler);

/*
 * set tSegment handler context
 */
void mve_set_handler_context(MVESTREAM *movie, void *context);

/*
 * play next chunk
 */
int32_t mve_play_next_chunk(MVESTREAM *movie);

void* MVE_Alloc (uint32_t size);
void MVE_Free(void* ptr);

#endif /* INCLUDED_MVELIB_H */
