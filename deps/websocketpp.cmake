message(STATUS "Downloading WebSocket++")

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
	websocketpp
  	URL https://github.com/zaphoyd/websocketpp/archive/refs/tags/0.8.2.zip
)

FetchContent_GetProperties(websocketpp)
if(NOT websocketpp_POPULATED)
  FetchContent_Populate(websocketpp)
endif()

message(STATUS "WebSocket++ downloaded.")

set(websocketpp_ROOT_DIR "${FETCHCONTENT_BASE_DIR}/websocketpp-src" CACHE STRING "WebSocketpp Root Directory")
set(websocketpp_INCLUDE_DIR 
	"${FETCHCONTENT_BASE_DIR}/websocketpp-src" 
	CACHE STRING "WebSocketpp Include File Location"
)
