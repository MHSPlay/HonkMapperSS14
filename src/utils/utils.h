#pragma once

struct tile_t
{
	std::string name;
	std::string texture_path;
	ID3D11ShaderResourceView* texture = nullptr;
	int width = 32;
	int height = 32;
};


class c_utils
{
public:
	std::string folder_path;
	std::string new_map_name;
	std::vector< tile_t > tiles;

	std::filesystem::path get_maps_folder( );
	std::filesystem::path get_textures_folder( );

	void update_maps( ID3D11Device* device );
	void load_tiles( ID3D11Device* device );
	bool load_texture_from_file( const char* filename, ID3D11Device* device, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height );

	void create_new_map( );
	void edit_exist_map( );

	void cleanup_textures( );
};

inline const std::unique_ptr< c_utils > g_utils{ new c_utils() };