/* libmpdclient
   (c) 2003-2008 The Music Player Daemon Project
   This project's homepage is: http://www.musicpd.org

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of the Music Player Daemon nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <mpd/status.h>
#include <mpd/pair.h>
#include <mpd/send.h>
#include "internal.h"

#include <stdlib.h>
#include <string.h>

void mpd_send_status(struct mpd_connection * connection) {
	mpd_send_command(connection, "status", NULL);
}

struct mpd_status * mpd_get_status(struct mpd_connection * connection) {
	struct mpd_status * status;

	/*mpd_send_command(connection, "status", NULL);

	if (connection->error) return NULL;*/

	if (connection->doneProcessing || (connection->listOks &&
	   connection->doneListOk))
	{
		return NULL;
	}

	if (connection->pair == NULL)
		mpd_getNextReturnElement(connection);

	if (mpd_error_is_defined(&connection->error))
		return NULL;

	status = malloc(sizeof(struct mpd_status));
	status->volume = -1;
	status->repeat = 0;
	status->random = 0;
	status->playlist = -1;
	status->playlist_length = -1;
	status->state = -1;
	status->song = 0;
	status->songid = 0;
	status->elapsed_time = 0;
	status->total_time = 0;
	status->bit_rate = 0;
	status->sample_rate = 0;
	status->bits = 0;
	status->channels = 0;
	status->crossfade = -1;
	status->error = NULL;
	status->updatingdb = 0;

	while (connection->pair != NULL) {
		const struct mpd_pair *pair = connection->pair;

		if (strcmp(pair->name, "volume") == 0) {
			status->volume = atoi(pair->value);
		}
		else if (strcmp(pair->name, "repeat") == 0) {
			status->repeat = atoi(pair->value);
		}
		else if (strcmp(pair->name, "random") == 0) {
			status->random = atoi(pair->value);
		}
		else if (strcmp(pair->name, "playlist") == 0) {
			status->playlist = strtol(pair->value,NULL,10);
		}
		else if (strcmp(pair->name, "playlistlength") == 0) {
			status->playlist_length = atoi(pair->value);
		}
		else if (strcmp(pair->name, "bitrate") == 0) {
			status->bit_rate = atoi(pair->value);
		}
		else if (strcmp(pair->name, "state") == 0) {
			if (strcmp(pair->value,"play") == 0) {
				status->state = MPD_STATUS_STATE_PLAY;
			}
			else if (strcmp(pair->value,"stop") == 0) {
				status->state = MPD_STATUS_STATE_STOP;
			}
			else if (strcmp(pair->value,"pause") == 0) {
				status->state = MPD_STATUS_STATE_PAUSE;
			}
			else {
				status->state = MPD_STATUS_STATE_UNKNOWN;
			}
		}
		else if (strcmp(pair->name, "song") == 0) {
			status->song = atoi(pair->value);
		}
		else if (strcmp(pair->name, "songid") == 0) {
			status->songid = atoi(pair->value);
		}
		else if (strcmp(pair->name, "time") == 0) {
			char * tok = strchr(pair->value,':');
			/* the second strchr below is a safety check */
			if (tok && (strchr(tok,0) > (tok+1))) {
				/* atoi stops at the first non-[0-9] char: */
				status->elapsed_time = atoi(pair->value);
				status->total_time = atoi(tok+1);
			}
		}
		else if (strcmp(pair->name, "error") == 0) {
			status->error = strdup(pair->value);
		}
		else if (strcmp(pair->name, "xfade") == 0) {
			status->crossfade = atoi(pair->value);
		}
		else if (strcmp(pair->name, "updating_db") == 0) {
			status->updatingdb = atoi(pair->value);
		}
		else if (strcmp(pair->name, "audio") == 0) {
			char * tok = strchr(pair->value,':');
			if (tok && (strchr(tok,0) > (tok+1))) {
				status->sample_rate = atoi(pair->value);
				status->bits = atoi(++tok);
				tok = strchr(tok,':');
				if (tok && (strchr(tok,0) > (tok+1)))
					status->channels = atoi(tok+1);
			}
		}

		mpd_getNextReturnElement(connection);
		if (mpd_error_is_defined(&connection->error)) {
			free(status);
			return NULL;
		}
	}

	if (mpd_error_is_defined(&connection->error)) {
		free(status);
		return NULL;
	}
	else if (status->state<0) {
		mpd_error_code(&connection->error, MPD_ERROR_MALFORMED);
		mpd_error_message(&connection->error, "state not found");
		free(status);
		return NULL;
	}

	return status;
}

void mpd_free_status(struct mpd_status * status) {
	if (status->error) free(status->error);
	free(status);
}

int mpd_status_get_volume(struct mpd_status * status)
{
	return status->volume;
}

int mpd_status_get_repeat(struct mpd_status * status)
{
        return status->repeat;
}

int mpd_status_get_random(struct mpd_status * status)
{
        return status->random;
}

int mpd_status_get_playlist_length(struct mpd_status * status)
{
        return status->playlist_length;
}

long long mpd_status_get_playlist(struct mpd_status * status)
{
        return status->playlist;
}

int mpd_status_get_state(struct mpd_status * status)
{
        return status->state;
}

int mpd_status_get_crossfade(struct mpd_status * status)
{
        return status->crossfade;
}

int mpd_status_get_song(struct mpd_status * status)
{
        return status->song;
}

int mpd_status_get_songid(struct mpd_status * status)
{
        return status->songid;
}

int mpd_status_get_elapsed_time(struct mpd_status * status)
{
        return status->elapsed_time;
}

int mpd_status_get_total_time(struct mpd_status * status)
{
        return status->total_time;
}

int mpd_status_get_bit_rate(struct mpd_status * status)
{
        return status->bit_rate;
}

unsigned int mpd_status_get_sample_rate(struct mpd_status * status)
{
        return status->sample_rate;
}

int mpd_status_get_bits(struct mpd_status * status)
{
        return status->bits;
}

int mpd_status_get_channels(struct mpd_status * status)
{
        return status->channels;
}

int mpd_status_get_updatingdb(struct mpd_status * status)
{
        return status->updatingdb;
}

char * mpd_status_get_error(struct mpd_status * status)
{
        return status->error;
}


