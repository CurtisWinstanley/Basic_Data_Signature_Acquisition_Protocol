message(STATUS "Downloading asio 1.24.")

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
	asio
	GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
	GIT_TAG "asio-1-24-0"
	GIT_PROGRESS TRUE
)

FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
  FetchContent_Populate(asio)
endif()

set(asio_INCLUDE_DIR 
	"${FETCHCONTENT_BASE_DIR}/asio-src/asio/include" 
	CACHE STRING "asio Include File Location"
)

message(STATUS "asio 1.24 downloaded.")