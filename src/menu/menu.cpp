#include "..\includes.h"

bool select_path( const char* buttonName, char* text, size_t bufferSize )
{
    if ( !ImGui::Button( buttonName ) ) 
        return false;
        
    if ( !text || bufferSize == NULL ) 
        return false;
     
    HRESULT init = CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
    if ( !SUCCEEDED( init ) )
        return false;
        
    _com_ptr_t< _com_IIID< IFileOpenDialog, &__uuidof( IFileOpenDialog )>> pFileOpen;

    HRESULT hr = CoCreateInstance( CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
        IID_IFileOpenDialog,
        reinterpret_cast< void** >( &pFileOpen ) );

    if ( FAILED( hr ) ) 
        return false;
        
    DWORD options;
    pFileOpen->GetOptions( &options );
    pFileOpen->SetOptions( options | FOS_PICKFOLDERS );

    hr = pFileOpen->Show( nullptr );
    if ( hr == HRESULT_FROM_WIN32( ERROR_CANCELLED ) ) 
        return false; 
        
    if ( FAILED( hr ) ) 
        return false;
        
    _com_ptr_t< _com_IIID< IShellItem, &__uuidof( IShellItem )>> pItem;
    hr = pFileOpen->GetResult( &pItem );
    if ( FAILED( hr ) ) 
        return false;
        
    PWSTR pszFilePath = nullptr;
    hr = pItem->GetDisplayName( SIGDN_FILESYSPATH, &pszFilePath );
    if ( FAILED( hr ) ) 
        return false;
        
    std::unique_ptr< wchar_t, decltype( &CoTaskMemFree )> filePathGuard( pszFilePath, CoTaskMemFree );

    int requiredSize = WideCharToMultiByte( CP_UTF8, 0, pszFilePath, -1, nullptr, 0, nullptr, nullptr );
    if ( requiredSize <= 0 || static_cast< size_t >( requiredSize ) > bufferSize )
        return false;
        
    int result = WideCharToMultiByte( CP_UTF8, 0, pszFilePath, -1, text, static_cast< int >( bufferSize ), nullptr, nullptr );

    return result > 0;
}

void c_menu::on_render()
{
    static ImVec2 window_size = ImVec2( 800, 600 );
    static ImVec2 window_pos = ImVec2( ( GetSystemMetrics( SM_CXSCREEN ) - 800 ) / 2, ( GetSystemMetrics( SM_CYSCREEN ) - 600 ) / 2 );
    static int menu_state = 0;
    static bool first_frame = true;
    
    if ( menu_state == 1 && first_frame )
    {
        window_size = ImVec2( static_cast< float >( GetSystemMetrics( SM_CXSCREEN ) ), static_cast< float >(GetSystemMetrics( SM_CYSCREEN ) ) );
        window_pos = ImVec2( 0, 0 );
        first_frame = false;
    }
    else if ( menu_state == 0 && !first_frame )
    {
        window_size = ImVec2( 800, 600 );
        window_pos = ImVec2( ( GetSystemMetrics( SM_CXSCREEN ) - 800 ) / 2, ( GetSystemMetrics( SM_CYSCREEN ) - 600 ) / 2 );
        first_frame = true;
    }

    ImGui::SetNextWindowPos( window_pos, ImGuiCond_Always );
    ImGui::SetNextWindowSize( window_size, ImGuiCond_Always );
	ImGui::Begin( "HonkMapper - github.com/MHSPlay/HonkMapperSS14", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
	{
        static bool open_popup_select_folder = false;
        static bool open_popup_new_map = false;

        if ( ImGui::BeginMenuBar( ) )
        {
            if ( ImGui::BeginMenu( "File" ) )
            {
                if ( ImGui::MenuItem( "New...", "Ctrl + N" ) )
                {
                    if ( g_utils->folder_path.empty( ) )
                        open_popup_select_folder = true;
                    else
                        open_popup_new_map = true;

                }
                if ( ImGui::MenuItem( "Open...", "Ctrl + O") )
                {

                }
                if ( ImGui::MenuItem( "Open Recent" ) ) 
                {

                }

                ImGui::Separator( );

                if ( ImGui::MenuItem( "Save", "Ctrl + S" ) )
                {

                }

                ImGui::Separator( );

                if ( ImGui::MenuItem( "Exit", "Ctrl + Q" ) )
                    exit( -1 );
                
                ImGui::EndMenu( );
            }

            if ( ImGui::BeginMenu( "Settings" ) )
            {
                if ( ImGui::MenuItem( "Change language" ) )
                {

                }

                ImGui::EndMenu( );
            }

            // more

            ImGui::EndMenuBar( );
        }

        if ( open_popup_select_folder )
        {
            ImGui::OpenPopup( "Warning #1" );
            open_popup_select_folder = false;
        }

        if ( open_popup_new_map )
        {
            ImGui::OpenPopup( "Creating a new map" );
            open_popup_new_map = false;
        }

        ImVec2 windowPos = ImGui::GetWindowPos( );
        ImVec2 windowSize = ImGui::GetWindowSize( );
        ImVec2 windowCenter = ImVec2( windowPos.x + windowSize.x * 0.5f, windowPos.y + windowSize.y * 0.5f );
        ImGui::SetNextWindowPos( windowCenter, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
        if ( ImGui::BeginPopupModal( "Warning #1", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
        {
            ImGui::Text( "Couldn't get a path to Space Station 14" );
            ImGui::Separator( );
            
            static char selectedPath[ MAX_PATH ] = "";
            char tempPath[ MAX_PATH ];
            if ( select_path( "Select", tempPath, MAX_PATH ) ) 
            {
                std::filesystem::path path( tempPath );
                if ( std::filesystem::exists( path / "Resources" ) )
                    strcpy_s( selectedPath, tempPath );

            }

            ImGui::SameLine( );
            
            if ( ImGui::Button( "Cancel" ) )
            {
                ImGui::CloseCurrentPopup( );
                selectedPath[ 0 ] = '\0';
            }
            
            if ( selectedPath[ 0 ] != '\0' )
            {
                g_utils->folder_path = selectedPath;
                ImGui::CloseCurrentPopup( );
                selectedPath[ 0 ] = '\0';
            }

            ImGui::EndPopup( );
        }

        ImGui::SetNextWindowPos( windowCenter, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
        if ( ImGui::BeginPopupModal( "Creating a new map", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
        {

            static char map_name_buffer[ MAX_PATH ] = "";
            ImGui::InputText( "Enter map name...", map_name_buffer, IM_ARRAYSIZE( map_name_buffer ) );

            ImGui::Separator( );

            if ( ImGui::Button( "Create" ) && strlen( map_name_buffer ) > 0)
            {
                g_utils->new_map_name = map_name_buffer;
                menu_state = 1;
                ImGui::CloseCurrentPopup( );
                map_name_buffer[ 0 ] = '\0';
            }
                
            ImGui::SameLine( );

            if ( ImGui::Button( "Cancel" ) )
                ImGui::CloseCurrentPopup( );

            ImGui::EndPopup( );
        }

        switch ( menu_state )
        {
            case 0: 
            {
            
                ImGui::Text( "Hello world!" );
            
            }    
            break;

            case 1:
            {
                static int selected_tile = -1;
                static char search_buffer[ 256 ] = "";

                ImGui::BeginChild( "LeftPanel_Tiles", ImVec2( 400, 0 ), true );
                ImGui::Text( "Tiles:" );

                ImGui::PushItemWidth( -1 );
                if ( ImGui::InputTextWithHint( "##search", "Search tiles...", search_buffer, IM_ARRAYSIZE( search_buffer ) ) )
                {
                    // При вводе текста сбрасываем выбор
                }
                ImGui::PopItemWidth();

                std::string search_str = search_buffer;
                std::transform( search_str.begin( ), search_str.end( ), search_str.begin( ), ::tolower );

                int visible_count = 0;

                ImGui::Separator( );
                {
                    for ( size_t i = 0; i < g_utils->tiles.size( ); i++ )
                    {
                        const auto& tile = g_utils->tiles[ i ];

                        if ( search_str.length( ) > 0 )
                        {
                            std::string tile_name = tile.name;
                            std::transform( tile_name.begin( ), tile_name.end( ), tile_name.begin( ), ::tolower );

                            if ( tile_name.find( search_str ) == std::string::npos )
                                continue;
                        }

                        visible_count++;

                        ImGui::PushID( static_cast< int >( i ) );

                        bool is_selected = ( selected_tile == static_cast< int >( i ) );
                        if (is_selected)
                        {
                            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.3f, 0.5f, 0.8f, 1.0f ) );
                            ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.4f, 0.6f, 0.9f, 1.0f ) );
                        }
                        else
                        {
                            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.15f, 0.15f, 0.15f, 1.0f ) );
                            ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.25f, 0.25f, 0.25f, 1.0f ) );
                        }

                        if ( ImGui::Button( "##tile_button", ImVec2( -1, 70 ) ) )
                            selected_tile = static_cast< int >( i );

                        ImGui::PopStyleColor( 2 );

                        ImVec2 button_pos = ImGui::GetItemRectMin( );
                        ImVec2 button_size = ImGui::GetItemRectSize( );
                        ImDrawList* draw_list = ImGui::GetWindowDrawList();

                        if ( tile.texture )
                        {
                            float max_size = 60.0f;
                            float scale = max_size / max( tile.width, tile.height );
                            ImVec2 img_size( tile.width * scale, tile.height * scale );

                            ImVec2 img_pos(
                                button_pos.x + 5,
                                button_pos.y + ( button_size.y - img_size.y ) * 0.5f
                            );

                            draw_list->AddImage(
                                ( void* )tile.texture,
                                img_pos,
                                ImVec2( img_pos.x + img_size.x, img_pos.y + img_size.y )
                            );

                            ImVec2 text_pos(
                                button_pos.x + max_size + 15,
                                button_pos.y + button_size.y * 0.5f - ImGui::GetTextLineHeight( ) * 0.5f
                            );

                            draw_list->AddText( text_pos, IM_COL32( 255, 255, 255, 255 ), tile.name.c_str( ) );
                        }
                        else
                        {
                            ImVec2 text_pos(
                                button_pos.x + 10,
                                button_pos.y + button_size.y * 0.5f - ImGui::GetTextLineHeight( ) * 0.5f
                            );

                            draw_list->AddText( text_pos, IM_COL32( 255, 255, 255, 255 ), tile.name.c_str( ) );
                        }

                        ImGui::PopID( );
                    }

                    if ( g_utils->tiles.empty( ) )
                    {
                        ImGui::TextDisabled( "No tiles found..." );
                        ImGui::TextDisabled( "Check Resources/Textures/Tiles folder" );
                    }
                    else if ( visible_count == 0 && search_str.length( ) > 0 )
                        ImGui::TextDisabled( "No tiles match search" );
                    
                }
                ImGui::EndChild( );

                ImGui::SameLine( );

                float centerWidth = ImGui::GetContentRegionAvail( ).x - 400;
                ImGui::BeginChild( "CenterPanel_ViewPort", ImVec2( centerWidth, 0 ), true );

                ImGui::Text( "ViewPort:" );
                ImGui::Separator( );
                {
                    ImVec2 viewport_pos = ImGui::GetCursorScreenPos( );
                    ImVec2 viewport_size = ImGui::GetContentRegionAvail( );

                    if ( selected_tile >= 0 && selected_tile < static_cast< int >( g_utils->tiles.size( ) ) )
                    {
                        const auto& tile = g_utils->tiles[ selected_tile ];

                        ImGui::Text( "Selected Tile:" );
                        ImGui::SameLine( );
                        ImGui::TextColored( ImVec4( 0.5f, 0.8f, 1.0f, 1.0f ), "%s", tile.name.c_str( ) );
                        ImGui::SameLine( );
                        ImGui::TextDisabled( "(Right click to deselect)" );
                        ImGui::Separator( );

                        // @todo: make grid zoomable / make grid coordinates / make checkbox for snap tile to grid
                        ImGui::BeginChild( "DrawArea", ImVec2( 0, 0 ), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
                        {
                            if ( ImGui::IsWindowHovered( ) && ImGui::IsMouseClicked( ImGuiMouseButton_Right ) )
                                selected_tile = -1;

                            ImVec2 canvas_pos = ImGui::GetCursorScreenPos( );
                            ImVec2 canvas_size = ImGui::GetContentRegionAvail( );

                            ImDrawList* draw_list = ImGui::GetWindowDrawList( );

                            const float grid_size = 64.0f;
                            ImU32 grid_color = IM_COL32( 50, 50, 50, 255 );

                            for ( float x = 0; x < canvas_size.x; x += grid_size )
                            {
                                draw_list->AddLine(
                                    ImVec2( canvas_pos.x + x, canvas_pos.y ),
                                    ImVec2( canvas_pos.x + x, canvas_pos.y + canvas_size.y ),
                                    grid_color
                                );
                            }

                            for ( float y = 0; y < canvas_size.y; y += grid_size )
                            {
                                draw_list->AddLine(
                                    ImVec2( canvas_pos.x, canvas_pos.y + y ),
                                    ImVec2( canvas_pos.x + canvas_size.x, canvas_pos.y + y ),
                                    grid_color
                                );
                            }

                            if ( ImGui::IsWindowHovered( ) )
                            {
                                ImVec2 mouse_pos = ImGui::GetMousePos( );

                                if ( tile.texture )
                                {
                                    static float tile_scale = 2.0f;
                                    ImVec2 tile_size( tile.width * tile_scale, tile.height * tile_scale );

                                    ImVec2 tile_pos(
                                        mouse_pos.x - tile_size.x * 0.5f,
                                        mouse_pos.y - tile_size.y * 0.5f
                                    );

                                    draw_list->AddRectFilled(
                                        tile_pos,
                                        ImVec2( tile_pos.x + tile_size.x, tile_pos.y + tile_size.y ),
                                        IM_COL32( 255, 255, 255, 50 )
                                    );

                                    draw_list->AddImage(
                                        ( void* )tile.texture,
                                        tile_pos,
                                        ImVec2( tile_pos.x + tile_size.x, tile_pos.y + tile_size.y ),
                                        ImVec2( 0, 0 ),
                                        ImVec2( 1, 1 ),
                                        IM_COL32( 255, 255, 255, 200 )
                                    );

                                    draw_list->AddRect(
                                        tile_pos,
                                        ImVec2( tile_pos.x + tile_size.x, tile_pos.y + tile_size.y ),
                                        IM_COL32( 100, 200, 255, 255 ),
                                        0.0f,
                                        0,
                                        2.0f
                                    );
                                }

                                // cursor position - @todo: change it on ss14 map position
                                ImVec2 relative_pos(
                                    mouse_pos.x - canvas_pos.x,
                                    mouse_pos.y - canvas_pos.y
                                );

                                char coord_text[ 64 ];
                                sprintf_s( coord_text, "X: %.0f Y: %.0f", relative_pos.x, relative_pos.y );

                                ImVec2 text_size = ImGui::CalcTextSize( coord_text );
                                ImVec2 text_pos(
                                    mouse_pos.x + 15,
                                    mouse_pos.y + 15
                                );

                                draw_list->AddRectFilled(
                                    ImVec2( text_pos.x - 3, text_pos.y - 3 ),
                                    ImVec2( text_pos.x + text_size.x + 3, text_pos.y + text_size.y + 3 ),
                                    IM_COL32( 0, 0, 0, 200 )
                                );

                                draw_list->AddText(
                                    text_pos,
                                    IM_COL32( 255, 255, 255, 255 ),
                                    coord_text
                                );
                            }
                        }
                        ImGui::EndChild( );
                    }
                    else // @todo: check have been placed tile?
                    {
                        ImVec2 region = ImGui::GetContentRegionAvail( );
                        ImVec2 text_size = ImGui::CalcTextSize( "Select a tile from the left panel" );

                        ImGui::SetCursorPos( ImVec2(
                            ( region.x - text_size.x ) * 0.5f,
                            ( region.y - text_size.y ) * 0.5f
                        ) );

                        ImGui::TextDisabled( "Select a tile from the left panel" );
                    }
                }
                ImGui::EndChild( );      

                ImGui::SameLine( );

                ImGui::BeginChild( "RightPanel_Container", ImVec2( 400, 0 ), true );
                {
                    ImGui::BeginChild( "RightPanel_Entities", ImVec2( 0, ImGui::GetContentRegionAvail( ).y * 0.5f ), true );
                    ImGui::Text( "Entities:" );
                    ImGui::Separator( );
                    {
                        for ( int i = 0; i < 5; i++ ) {
                            ImGui::Text( "Entity %d", i );
                        }
                    }
                    ImGui::EndChild( );

                    ImGui::Spacing( );

                    ImGui::BeginChild( "RightPanel_Decals", ImVec2( 0, ImGui::GetContentRegionAvail( ).y ), true );
                    ImGui::Text( "Decals:" );
                    ImGui::Separator( );
                    {
                        for ( int i = 0; i < 5; i++ ) {
                            ImGui::Text( "Decal %d", i );
                        }
                    }
                    ImGui::EndChild( );
                }
                ImGui::EndChild( );

            }    
            break;
        }

	}
	ImGui::End( );

}
