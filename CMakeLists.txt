cmake_minimum_required(VERSION 3.15)

project(libmpdclient C)

set(PACKAGE "libmpdclient")
set(VERSION "2.18")
set(DEFAULT_SOCKET "/var/run/mpd/socket")
set(DEFAULT_HOST "localhost")
set(DEFAULT_PORT 6600)
set(HAVE_STRNDUP TRUE)
set(TCP_ENABLE TRUE)
set(HAVE_GETADDRINFO TRUE)

configure_file(
	include/mpd/config.h.in
	config.h
)

set(MAJOR_VERSION 2)
set(MINOR_VERSION 18)
set(PATCH_VERSION 0)

configure_file(
	include/mpd/version.h.in
	version.h
)

add_library(mpdclient
	src/async.c
	src/audio_format.c
	src/buffer.h
	src/capabilities.c
	src/cmessage.c
	src/cmount.c
	src/cneighbor.c
	src/connection.c
	src/coutput.c
	src/cpartition.c
	src/cplaylist.c
	src/cstats.c
	src/cstatus.c
	src/database.c
	src/directory.c
	src/entity.c
	src/error.c
	src/example.c
	src/fd_util.c
	src/fd_util.h
	src/fingerprint.c
	src/iaf.h
	src/iasync.h
	src/idle.c
	src/ierror.c
	src/ierror.h
	src/internal.h
	src/isend.h
	src/iso8601.c
	src/iso8601.h
	src/kvlist.c
	src/kvlist.h
	src/list.c
	src/message.c
	src/mixer.c
	src/mount.c
	src/neighbor.c
	src/output.c
	src/parser.c
	src/partition.c
	src/password.c
	src/player.c
	src/playlist.c
	src/queue.c
	src/quote.c
	src/quote.h
	src/rdirectory.c
	src/recv.c
	src/replay_gain.c
	src/resolver.c
	src/resolver.h
	src/response.c
	src/rplaylist.c
	src/run.c
	src/run.h
	src/search.c
	src/send.c
	src/settings.c
	src/socket.c
	src/socket.h
	src/song.c
	src/stats.c
	src/status.c
	src/sticker.c
	src/sync.c
	src/sync.h
	src/tag.c
	src/uri.h
	include/mpd/async.h
	include/mpd/audio_format.h
	include/mpd/capabilities.h
	include/mpd/client.h
	include/mpd/compiler.h
	include/mpd/connection.h
	include/mpd/database.h
	include/mpd/directory.h
	include/mpd/entity.h
	include/mpd/error.h
	include/mpd/fingerprint.h
	include/mpd/idle.h
	include/mpd/list.h
	include/mpd/message.h
	include/mpd/mixer.h
	include/mpd/mount.h
	include/mpd/neighbor.h
	include/mpd/output.h
	include/mpd/pair.h
	include/mpd/parser.h
	include/mpd/partition.h
	include/mpd/password.h
	include/mpd/player.h
	include/mpd/playlist.h
	include/mpd/protocol.h
	include/mpd/queue.h
	include/mpd/recv.h
	include/mpd/replay_gain.h
	include/mpd/response.h
	include/mpd/search.h
	include/mpd/send.h
	include/mpd/settings.h
	include/mpd/socket.h
	include/mpd/song.h
	include/mpd/stats.h
	include/mpd/status.h
	include/mpd/sticker.h
	include/mpd/tag.h
	)

target_include_directories(mpdclient
	PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}"
	PRIVATE src .
	)

target_compile_options(mpdclient PRIVATE
	-Wall
	-Wextra
	-Wno-deprecated-declarations
	-Wshadow
	-Wpointer-arith
	-Wmissing-prototypes
	-Wstrict-prototypes
	-Wcast-qual
	-Wwrite-strings
	)