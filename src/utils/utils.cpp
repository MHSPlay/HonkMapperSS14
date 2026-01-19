#include "..\includes.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\..\third-party\stb_image\stb_image.h"

bool c_utils::load_texture_from_file( const char* filename, ID3D11Device* device, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height )
{
	int width, height, channels;
	unsigned char* data = stbi_load( filename, &width, &height, &channels, 4 );
	if ( !data )
		return false;

	D3D11_TEXTURE2D_DESC desc = { };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* texture = nullptr;
	D3D11_SUBRESOURCE_DATA subResource = { };
	subResource.pSysMem = data;
	subResource.SysMemPitch = width * 4;
	subResource.SysMemSlicePitch = 0;
	HRESULT hr = device->CreateTexture2D( &desc, &subResource, &texture );

	stbi_image_free( data );

	if ( FAILED( hr ) )
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView( texture, &srvDesc, out_srv );
	texture->Release( );

	if ( FAILED( hr ) )
		return false;

	*out_width = width;
	*out_height = height;

	return true;
}

std::filesystem::path c_utils::get_maps_folder( )
{
	std::filesystem::path basic_path( folder_path );
    std::filesystem::path maps_path = basic_path / "Resources" / "Maps";
    
    if ( std::filesystem::exists( maps_path ) )
        return maps_path;
 
    return "";
}

std::filesystem::path c_utils::get_textures_folder( )
{
	std::filesystem::path basic_path( folder_path );
	std::filesystem::path textures_path = basic_path / "Resources" / "Textures";

	if ( std::filesystem::exists( textures_path ) )
		return textures_path;

	return "";
}

void c_utils::load_tiles( ID3D11Device* device )
{
	if ( !device )
		return;

	cleanup_textures( );
	tiles.clear( );

	std::filesystem::path textures = get_textures_folder( );
	if ( textures.empty( ) )
		return;

	std::filesystem::path tiles_path = textures / "Tiles";
	if ( !std::filesystem::exists( tiles_path ) )
		return;

	for ( const auto& entry : std::filesystem::recursive_directory_iterator( tiles_path ) )
	{
		if ( !entry.is_regular_file( ) )
			continue;

		std::string extension = entry.path( ).extension( ).string( );
		std::transform( extension.begin( ), extension.end( ), extension.begin( ), ::tolower );

		if ( extension == ".png" )
		{
			tile_t tile;
			tile.name = entry.path( ).stem( ).string( );
			tile.texture_path = entry.path( ).string( );

			if ( load_texture_from_file( tile.texture_path.c_str( ), device, &tile.texture, &tile.width, &tile.height ) )
				tiles.push_back( tile );
			
		}
	}
}

void c_utils::cleanup_textures( )
{
	for ( auto& tile : tiles )
	{
		if ( tile.texture )
		{
			tile.texture->Release( );
			tile.texture = nullptr;
		}
	}
}

void c_utils::update_maps( ID3D11Device* device )
{
    if ( folder_path.empty( ) )
        return;

    std::filesystem::path maps = get_maps_folder( );
    if ( maps.empty( ) )
        return;
    
    for ( const auto& entry : std::filesystem::directory_iterator( maps ) )
    {
        // inside the folders
        if ( entry.is_directory( ) ) 
        {
            for ( const auto& file_entry : std::filesystem::directory_iterator( entry.path( ) ) ) 
            {
                if ( !file_entry.is_regular_file( ) )
                    continue;

                std::string extension = file_entry.path( ).extension( ).string( );
                std::transform( extension.begin( ), extension.end( ), extension.begin( ), ::tolower );

                if ( extension == ".yml" )
                {

                }

            }

            continue;

        }

        std::string extension = entry.path( ).extension( ).string( );
        std::transform( extension.begin( ), extension.end( ), extension.begin( ), ::tolower );

        if ( extension == ".yml" )
        {



        }

    }

}
    
void c_utils::create_new_map(  )
{



}
