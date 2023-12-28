add_executable(apertium-simpleton
	common.hpp
	installer.cpp
	installer.hpp
	installer.ui
	main.cpp
	simpleton.cpp
	simpleton.hpp
	simpleton.ui
	)

target_link_libraries(apertium-simpleton ${QT_LIBS})

install(TARGETS apertium-simpleton RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
