#pragma once

class c_menu
{
public:

	void on_render( );

};

inline const std::unique_ptr< c_menu > g_menu{ new c_menu() };