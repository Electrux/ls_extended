project( ls_extended ) {
	language( c, 11 )
	version( 2.0.0 )
	license( bsd3 )
	author( Electrux, "ElectruxRedsworth@gmail.com" )
}

builds.add_flags( "-O2" )

builds( bin ) {
	sources( "src/(.*)\.c" )
	build( ls_extended, "src/main.c" )
}
