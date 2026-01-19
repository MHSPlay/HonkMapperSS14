#include "..\includes.h"

std::filesystem::path c_utils::get_maps_folder( )
{
	std::filesystem::path basic_path( folder_path );
    std::filesystem::path maps_path = basic_path / "Resources" / "Maps";
    
    if ( std::filesystem::exists( maps_path ) )
        return maps_path;
 
    return "";
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
