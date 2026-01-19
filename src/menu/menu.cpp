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

                ImGui::BeginChild( "LeftPanel_Tiles", ImVec2( 250, 0 ), true );
                ImGui::Text( "Tiles:" );
                ImGui::Separator( );
                {
                    for ( int i = 0; i < 10; i++ ) {
                        if ( ImGui::Selectable( ( "Item " + std::to_string( i ) ).c_str( ) ) ) {

                        }
                    }
                }
                ImGui::EndChild( );

                ImGui::SameLine( );

                float centerWidth = ImGui::GetContentRegionAvail( ).x - 400;
                ImGui::BeginChild( "CenterPanel_ViewPort", ImVec2( centerWidth, 0 ), true );
                ImGui::Text( "ViewPort:" );
                ImGui::Separator( );
                {
                    ImGui::Text( "Main content area" );
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
