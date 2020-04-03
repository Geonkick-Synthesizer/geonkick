set(GKICK_API_HEADERS
	${GKICK_API_DIR}/src/audio_output.h
	${GKICK_API_DIR}/src/compressor.h
	${GKICK_API_DIR}/src/distortion.h
	${GKICK_API_DIR}/src/envelope.h
	${GKICK_API_DIR}/src/filter.h
	${GKICK_API_DIR}/src/geonkick.h
	${GKICK_API_DIR}/src/geonkick_internal.h
	${GKICK_API_DIR}/src/gkick_audio.h
        ${GKICK_API_DIR}/src/mixer.h
	${GKICK_API_DIR}/src/gkick_buffer.h
	${GKICK_API_DIR}/src/gkick_jack.h
	${GKICK_API_DIR}/src/gkick_log.h
	${GKICK_API_DIR}/src/oscillator.h
	${GKICK_API_DIR}/src/synthesizer.h)

set(GKICK_API_SOURCES
	${GKICK_API_DIR}/src/audio_output.c
	${GKICK_API_DIR}/src/compressor.c
	${GKICK_API_DIR}/src/distortion.c
	${GKICK_API_DIR}/src/envelope.c
	${GKICK_API_DIR}/src/filter.c
	${GKICK_API_DIR}/src/geonkick.c
	${GKICK_API_DIR}/src/gkick_audio.c
        ${GKICK_API_DIR}/src/mixer.c
	${GKICK_API_DIR}/src/gkick_buffer.c
	${GKICK_API_DIR}/src/gkick_jack.c
	${GKICK_API_DIR}/src/gkick_log.c
	${GKICK_API_DIR}/src/oscillator.c
	${GKICK_API_DIR}/src/synthesizer.c)

include_directories(${GKICK_API_DIR}/src)

if (GKICK_STANDALONE)
	add_library(api_standalone STATIC
		${GKICK_API_HEADERS}
		${GKICK_API_SOURCES})
	target_compile_options(api_standalone PUBLIC ${GKICK_API_FLAGS})
endif (GKICK_STANDALONE)

if (GKICK_PLUGIN)
	add_library(api_plugin STATIC
		${GKICK_API_HEADERS}
		${GKICK_API_SOURCES})
	target_compile_options(api_plugin PUBLIC ${GKICK_API_PLUGIN_FLAGS})
endif (GKICK_PLUGIN)

