#pragma once

class c_utils
{
public:
	std::string folder_path;
	std::string new_map_name;

	std::filesystem::path get_maps_folder( );

	void update_maps( ID3D11Device* device );

	void create_new_map( );

	void edit_exist_map( );

};

inline const std::unique_ptr< c_utils > g_utils{ new c_utils() };