

project(libice)
cmake_minimum_required(VERSION 2.8)
MESSAGE(STATUS  "####################################")
MESSAGE(STATUS  "#############${PROJECT_NAME}###############")
MESSAGE(STATUS  "####################################")

#add_definitions(
				#	-DWIN32
				#	-D_DEBUG
				##	-D_WINDOWS
				#	-D_UNICODE
				#	-DUNICODE
				#	-D_AFXDLL
#)

include_directories(
	#${XRTC_DIR}
	#${XRTC_THIRD_PARTY_DIR}/include
	#${XRTC_DIR}/../xrtc-third_party/NIM_Duilib_Framework
	.
	../libwebrtc
	../deps/libyuv/include
	../deps/abseil-cpp/
	../deps/curl-7.81.0/include
	../deps/jsoncpp/source/include
)

link_directories(
#	${XRTC_THIRD_PARTY_DIR}/lib
#	${XRTC_DIR}/../xrtc-third_party/out/Windows/lib
)


#

file(GLOB ice_server_main_source 
	 
	ice_server_main.cpp
	 
) 


file(GLOB ice_client_main_source 
	 
	ice_client_main.cpp
	 
) 
add_executable(ice_server_main ${ice_server_main_source})
add_executable(ice_client_main ${ice_client_main_source})


set(ALL_LIB 

libwebrtc.lib
		abseil_dll.lib
		absl_cordz_functions.lib
		absl_cordz_handle.lib
		absl_cordz_info.lib
		absl_cordz_sample_token.lib
		absl_flags.lib
		absl_flags_commandlineflag_internal.lib
		absl_flags_internal.lib
		absl_flags_marshalling.lib
		absl_flags_parse.lib
		absl_flags_private_handle_accessor.lib
		absl_flags_program_name.lib
		absl_flags_reflection.lib
		absl_flags_usage.lib
		absl_flags_usage_internal.lib
		absl_low_level_hash.lib
		absl_random_internal_distribution_test_util.lib
		jpeg-static.lib
		

		#curl 
		jsoncpp_static.lib
		libcurl.lib
		# curl 
		Ws2_32.lib
		Wldap32.lib
		# api
		audio_options_api
		
		api.lib
		peerconnection.lib

		#  moudles /video /desktop_capture

		desktop_capture_generic.lib
		primitives
		desktop_capture_differ_sse2

		d3d11.lib
      dxgi.lib
		

		#common_video 

		common_video
		
		proxy.lib
		rtc_pc_base.lib
		checks.lib
		rtc_base.lib
		common_audio.lib
		task_queue.lib
		default_task_queue_factory.lib
		rtc_base_win.lib
		rtc_base_win_wgc.lib
		rtc_base_approved.lib

		rtc_media_base.lib



		# api/video

		video_rtp_headers 						
		video_frame 							
		video_frame_i010    					
		#recordable_encoded_frame 				
		#video_frame_type  						
		#render_resolution 						
		encoded_image 							
		encoded_frame 							
		rtp_video_frame_assembler 				
		#video_codec_constants  				
		video_bitrate_allocation 				
		#video_layers_allocation 				
		video_bitrate_allocator 				
		#video_bitrate_allocator_factory 		
		#video_stream_decoder 					
		video_stream_decoder_create 			
		video_adaptation 						
		#video_stream_encoder 					
		video_frame_metadata 					
		builtin_video_bitrate_allocator_factory


		# api 
		media_stream_interface

			Imm32 
			comctl32
			ws2_32.lib
			winmm.lib


			libyuv.lib
		libcrypto.lib 
		libssl.lib 
		#
		##libredis.lib
		#libzmalloc.lib
		iphlpapi.lib
		#crypto32.lib 
		ws2_32   
		crypt32
		Shell32.lib
		 advapi32
              iphlpapi
              psapi
              shell32
              user32
              userenv
              ws2_32
			  mswsock.lib
			  #  webrtc  timeGetTime  
			  winmm.lib
			  #ucrt.lib
			  #  libuv.lib  的依赖库
			  dbghelp.lib
			  			  rtc_p2p 
			  p2p_server_utils
			  libstunprober


			  # rtc_base/third_party
		base64.lib
		sigslot.lib 
		# rtc_base/system
		file_wrapper.lib
		# rtc_base/synchronization
		yield                       
		mutex               		
		sequence_checker_internal 	
		yield_policy

		#rtc_base
		callback_list
		rtc_numerics
		logging
		ip_address
		socket_address
		 crypt32.lib
		iphlpapi.lib
       secur32.lib
		#rtc_base
		weak_ptr
		network_constants
		async_resolver_interface
		async_socket            
		null_socket_server      
		threading               
		socket 
		stringutils
		timeutils
		platform_thread
		platform_thread_types
		net_helpers
		rtc_event
		win32 
		rtc_base
		#api
		wrapping_async_dns_resolver
		libjingle_peerconnection_api
		#async_dns_resolver
		#packet_socket_factory
		#scoped_refptr

		#api
		rtc_error
		#api /rtc_event_log 
		rtc_event_log
		rtc_event_log_factory

		# logging

		#rtc_event_log_api  		   		
		rtc_event_field  		   		
		rtc_stream_config  		   		
		rtc_event_pacing  		   		
		rtc_event_audio  		   		
		rtc_event_bwe  		   			
		rtc_event_frame_events  		
		rtc_event_generic_packet_events 
		rtc_event_rtp_rtcp  		   	
		rtc_event_video      		   	
		rtc_event_number_encodings  	
		rtc_event_log_impl_encoder  	
		#rtc_event_log_impl_output  		
		fake_rtc_event_log  		   	
		ice_log  	
		
		#rtc_base/experiments 
		#alr_experiment                             
		field_trial_parser                         
		quality_rampup_experiment                  
		quality_scaler_settings                    
		bandwidth_quality_scaler_settings          
		quality_scaling_experiment                 
		normalize_simulcast_size_experiment        
		balanced_degradation_settings              
		cpu_speed_experiment                       
		encoder_info_settings                      
		rtt_mult_experiment                        
		jitter_upper_bound_experiment              
		rate_control_settings                      
		keyframe_interval_settings_experiment      
		stable_target_rate_experiment              
		min_video_bitrate_experiment 
		# rtc_base
		

)

target_link_libraries(ice_server_main  ${ALL_LIB} )


target_link_libraries(ice_client_main  ${ALL_LIB} )


#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /SAFESEH:NO")


set(AllFile ${ice_client_main_source}
			${ice_server_main_source}
)
foreach(fileItem ${AllFile})       
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
	
    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")
	#message(status   "---> fileItem = ${fileItem}     GROUP ---->  ${GROUP}")
    # Group into "Source Files" and "Header Files"
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${fileItem}")
endforeach()



set_property(TARGET ice_client_main  				PROPERTY FOLDER ${PROJECT_NAME})
set_property(TARGET ice_server_main  				PROPERTY FOLDER ${PROJECT_NAME})

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})