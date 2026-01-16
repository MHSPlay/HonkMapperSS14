#include "..\includes.h"

void c_menu::on_render()
{

	ImGui::SetNextWindowSize( ImVec2( 800,600 ), ImGuiCond_Once );
	ImGui::Begin( "HonkMapper - github.com/MHSPlay/HonkMapperSS14", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse );
	{
        if ( ImGui::BeginMenuBar( ) )
        {
            if ( ImGui::BeginMenu( "File" ) )
            {
                if ( ImGui::MenuItem( "New" ) )
                { 

                }
                if ( ImGui::MenuItem( "Open" ) ) 
                {

                }

                ImGui::EndMenu( );
            }

            if ( ImGui::BeginMenu( "Settings" ) )
            {
                if ( ImGui::MenuItem( "Update ss14 folder" ) )
                { 

                }

                ImGui::EndMenu( );
            }

            // more

            ImGui::EndMenuBar( );
        }
		
		
	}
	ImGui::End( );

}
