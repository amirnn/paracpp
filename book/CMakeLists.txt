# book

find_package(LATEX REQUIRED)

# The directory where the final build data will be stored.
set(LATEX_OUTPUT_PATH book/build)
set(OUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${LATEX_OUTPUT_PATH}")
# Directory where the source files are located.
set( WORKINGDIR "${CMAKE_SOURCE_DIR}/book/src" )

# Latex source file.
set( MAIN_TEX_BASE_FILENAME "main")
set( MAIN_TEX "${WORKINGDIR}/${MAIN_TEX_BASE_FILENAME}.tex")

if(LATEX_FOUND AND LATEX_PDFLATEX_FOUND)
  # First pass.
  add_custom_target( latex-prebuild
    COMMAND ${PDFLATEX_COMPILER}  -output-directory ${OUT_DIRECTORY} -draftmode -interaction=nonstopmode ${MAIN_TEX}
    COMMENT "Starting Prebuild."
    WORKING_DIRECTORY ${WORKINGDIR}
    DEPENDS ${MAIN_TEX}
  )

  # Second pass - generate the final pdf.
  add_custom_target( latex-pdf	
    COMMAND ${PDFLATEX_COMPILER} -output-directory ${OUT_DIRECTORY}  ${MAIN_TEX}
    WORKING_DIRECTORY ${WORKINGDIR}
    COMMENT "Assembling the final pdf file."
    DEPENDS  ${MAIN_TEX}
  )
  add_custom_target(all-formats ALL) # Entry point of execution.
  add_dependencies( all-formats latex-pdf)

else()
  message(ERROR "No latex tools found!")
endif()