project( ls_extended ) {
	version( 1.0.8 )
	language( c, 11 )
	license( bsd3 )
	author( Electrux, "ElectruxRedsworth@gmail.com" )
}

builds.add_c_flags( "-O2" )

builds( bin ) {
	sources( "src/(.*)\.c" )
	build( ls_extended, "src/main.c" )
}

builds( test ) {
	sources( "src/(.*)\.c", "-src/main.c" )
	build( colored_display_variadic_params, "tests/colored_display_variadic_params.c" )
	build( colored_display_colors, "tests/colored_display_colors.c" )
	build( colored_display_invalid_colors, "tests/colored_display_invalid_colors.c" )
	build( colored_display_padding, "tests/colored_display_padding.c" )
	build( strvec_insert, "tests/strvec_insert.c" )
	build( strvec_delete, "tests/strvec_delete.c" )
	build( strvec_get, "tests/strvec_get.c" )
	build( strvec_count, "tests/strvec_count.c" )
	build( strvec_all, "tests/strvec_all.c" )
	runtests()
}
