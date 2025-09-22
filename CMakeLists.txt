

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
add_subdirectory(fake_ice_transport)
add_subdirectory(fake_dtls_transport)
add_subdirectory(fake_port_allocator)
add_subdirectory(fake_packet_transport)
add_subdirectory(fake_stun)
add_subdirectory(ice)
add_subdirectory(p2p_peerconnection)
add_subdirectory(libmedia)
#

file(GLOB ice_server_main_source 
	 
	ice_server_main.cpp
	cjsep_transport_controller_test.h 
	cjsep_transport_controller_test.cpp 
	 
) 


file(GLOB ice_client_main_source 
	 
	ice_client_main.cpp
	cjsep_transport_controller_test.h 
	cjsep_transport_controller_test.cpp 
	 
) 


FILE(GLOB  fake_stun_test_source


	fake_stun_test.cpp 
	)
	
	
FILE(GLOB  libice_main_source
	libice_main.cpp 
)

add_executable(ice_server_main ${ice_server_main_source})
add_executable(ice_client_main ${ice_client_main_source})
add_executable(fake_stun_test   ${fake_stun_test_source})
add_executable(libice_main   ${libice_main_source})

set(ALL_LIB 

		libwebrtc
		abseil_dll 
		absl_cord_internal 
		absl_cordz_functions 
		absl_cordz_handle 
		absl_cordz_info 
		absl_cordz_sample_token 
		absl_flags 
		absl_flags_commandlineflag_internal 
		absl_flags_internal 
		absl_flags_marshalling 
		absl_flags_parse 
		absl_flags_private_handle_accessor 
		absl_flags_program_name 
		absl_flags_reflection 
		absl_flags_usage 
		absl_flags_usage_internal 
		absl_low_level_hash 
		absl_random_internal_distribution_test_util 
		absl_statusor 

		absl_cord_internal 
		absl_cordz_functions 
		absl_cordz_handle 
		absl_cordz_info 
		absl_cordz_sample_token 
		absl_flags 
		absl_flags_commandlineflag 
		absl_flags_commandlineflag_internal 
		absl_flags_config 
		absl_flags_internal 
		absl_flags_marshalling 
		absl_flags_parse 
		absl_flags_private_handle_accessor 
		absl_flags_program_name 
		absl_flags_reflection 
		absl_flags_usage 
		absl_flags_usage_internal 
		absl_low_level_hash 
		absl_random_internal_distribution_test_util 
		absl_statusor 
		absl_strerror 



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
		# api/ numerics
		numerics
		# api/video_codecs 
		video_codecs_api  				
		#bitstream_parser_api  			
		builtin_video_decoder_factory  	
		builtin_video_encoder_factory  	
		vp8_temporal_layers_factory  	
		rtc_software_fallback_wrappers  
		#  moudles /video /desktop_capture

		desktop_capture_generic.lib
		primitives
		desktop_capture_differ_sse2

		d3d11.lib
      dxgi.lib
		

		#common_video 

		common_video
		
		proxy.lib
		rtc_pc_base
		checks.lib
		rtc_base.lib
		common_audio.lib
		task_queue.lib
		default_task_queue_factory.lib
		rtc_base_win.lib
		rtc_base_win_wgc.lib
		rtc_base_approved.lib

		rtc_media_base.lib


		libsrtp2.lib
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
		#rtc_base/memory 
		aligned_malloc  
		fifo_buffer  	
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
		rtc_base_tests_utils
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

		#call 

		version                  
		call_interfaces          
		#audio_sender_interface   
		rtp_interfaces           
		call_rtp_receiver             
		call_rtp_sender               
		bitrate_configurator     
		bitrate_allocator        
		call                     
		#receive_stream_interface 
		video_stream_api         
		simulated_network        
		fake_network   
		
		#pc 
		session_description  				
		simulcast_description  				
		media_protocol_names  				
		connection_context  				
		peer_connection_message_handler  	
		usage_pattern  						
		rtp_transceiver  					
		rtp_transmission_manager  			
		transceiver_list  					
		pc_rtp_receiver  						
		audio_rtp_receiver  				
		video_rtp_receiver  				
		video_rtp_track_source  			
		audio_track  						
		video_track  						
		#sdp_state_provider  				
		jitter_buffer_delay  				
		remote_audio_source  				
		pc_rtp_sender  						
		rtp_parameters_conversion  			
		dtmf_sender  						
		media_stream  						
		video_track_source  				
		#stats_collector_interface            
		
		

		rtc_sdp_video_format_utils


		# api/audio_codecs

		audio_codecs_api 				
		rtc_sdp_video_format_utils 	
		builtin_audio_decoder_factory 	
		builtin_audio_encoder_factory 	
		opus_audio_decoder_factory 	
		opus_audio_encoder_factory 	

		audio_encoder_g711 
		 audio_decoder_g711 
		 audio_encoder_g722 
		  audio_decoder_g722 
		  audio_encoder_ilbc 
		   audio_decoder_ilbc 

		  # audio_encoder_isac 		
		 #   audio_decoder_isac 		
		    audio_encoder_isac_float 	
		   
		    audio_decoder_isac_float 	
		audio_decoder_L16 		
		 audio_encoder_L16 	

		 audio_encoder_opus_config 	
		 #audio_decoder_opus_config 	
		 audio_encoder_opus 		
		 audio_decoder_opus 		
		 audio_encoder_multiopus 	
		 audio_decoder_multiopus 	


		 #modules/audio_coding 

		 #audio_coding_module_typedefs 	
		 audio_coding 					
		 legacy_encoded_audio_frame 	
		 webrtc_cng 					
		 audio_encoder_cng 				
		 red 							
		 g711 							
		 g711_c 						
		 g722 							
		 g722_c 						
		 ilbc 							
		 ilbc_c 						
		# isac_common 					
		 isac 							
		# isac_bwinfo 					
		 isac_vad 						
		 isac_fix 						
		 isac_fix_common 				
		 isac_fix_c 					
		 pcm16b 						
		 pcm16b_c 						
		 audio_coding_opus_common 		
		 webrtc_opus 					
		 webrtc_multiopus 				
		 webrtc_opus_wrapper 			
		 audio_network_adaptor_config 	
		 audio_network_adaptor 			
		 neteq 							
		 default_neteq_factory 			
		 neteq_tools_minimal 			
		 neteq_test_tools 				
		 neteq_tools 					
		 neteq_input_audio_tools 		
		 #audio_decoder_interface 		
		# audio_encoder_interface 		
		fake_port_allocator
		fake_packet_transport
		fake_dtls_transport 
		fake_ice_transport
		fake_stun
		
		

)


set(ALL_LIBICE 

		libwebrtc
		abseil_dll 
		absl_cord_internal 
		absl_cordz_functions 
		absl_cordz_handle 
		absl_cordz_info 
		absl_cordz_sample_token 
		absl_flags 
		absl_flags_commandlineflag_internal 
		absl_flags_internal 
		absl_flags_marshalling 
		absl_flags_parse 
		absl_flags_private_handle_accessor 
		absl_flags_program_name 
		absl_flags_reflection 
		absl_flags_usage 
		absl_flags_usage_internal 
		absl_low_level_hash 
		absl_random_internal_distribution_test_util 
		absl_statusor 

		absl_cord_internal 
		absl_cordz_functions 
		absl_cordz_handle 
		absl_cordz_info 
		absl_cordz_sample_token 
		absl_flags 
		absl_flags_commandlineflag 
		absl_flags_commandlineflag_internal 
		absl_flags_config 
		absl_flags_internal 
		absl_flags_marshalling 
		absl_flags_parse 
		absl_flags_private_handle_accessor 
		absl_flags_program_name 
		absl_flags_reflection 
		absl_flags_usage 
		absl_flags_usage_internal 
		absl_low_level_hash 
		absl_random_internal_distribution_test_util 
		absl_statusor 
		absl_strerror 



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
		# api/ numerics
		numerics
		# api/video_codecs 
		video_codecs_api  				
		#bitstream_parser_api  			
		builtin_video_decoder_factory  	
		builtin_video_encoder_factory  	
		vp8_temporal_layers_factory  	
		rtc_software_fallback_wrappers  
		#  moudles /video /desktop_capture

		desktop_capture_generic.lib
		primitives
		desktop_capture_differ_sse2

		d3d11.lib
      dxgi.lib
		

		#common_video 

		common_video
		
		proxy.lib
		rtc_pc_base
		checks.lib
		rtc_base.lib
		common_audio.lib
		task_queue.lib
		default_task_queue_factory.lib
		rtc_base_win.lib
		rtc_base_win_wgc.lib
		rtc_base_approved.lib

		rtc_media_base.lib


		libsrtp2.lib
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
			#  rtc_p2p 
			 # p2p_server_utils
			 # libstunprober


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
		#rtc_base/memory 
		aligned_malloc  
		fifo_buffer  	
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
		rtc_base_tests_utils
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

		#call 

		version                  
		call_interfaces          
		#audio_sender_interface   
		rtp_interfaces           
		call_rtp_receiver             
		call_rtp_sender               
		bitrate_configurator     
		bitrate_allocator        
		call                     
		#receive_stream_interface 
		video_stream_api         
		simulated_network        
		fake_network   
		
		#pc 
		session_description  				
		simulcast_description  				
		media_protocol_names  				
		connection_context  				
		peer_connection_message_handler  	
		usage_pattern  						
		rtp_transceiver  					
		rtp_transmission_manager  			
		transceiver_list  					
		pc_rtp_receiver  						
		audio_rtp_receiver  				
		video_rtp_receiver  				
		video_rtp_track_source  			
		audio_track  						
		video_track  						
		#sdp_state_provider  				
		jitter_buffer_delay  				
		remote_audio_source  				
		pc_rtp_sender  						
		rtp_parameters_conversion  			
		dtmf_sender  						
		media_stream  						
		video_track_source  				
		#stats_collector_interface            
		
		

		rtc_sdp_video_format_utils


		# api/audio_codecs

		audio_codecs_api 				
		rtc_sdp_video_format_utils 	
		builtin_audio_decoder_factory 	
		builtin_audio_encoder_factory 	
		opus_audio_decoder_factory 	
		opus_audio_encoder_factory 	

		audio_encoder_g711 
		 audio_decoder_g711 
		 audio_encoder_g722 
		  audio_decoder_g722 
		  audio_encoder_ilbc 
		   audio_decoder_ilbc 

		  # audio_encoder_isac 		
		 #   audio_decoder_isac 		
		    audio_encoder_isac_float 	
		   
		    audio_decoder_isac_float 	
		audio_decoder_L16 		
		 audio_encoder_L16 	

		 audio_encoder_opus_config 	
		 #audio_decoder_opus_config 	
		 audio_encoder_opus 		
		 audio_decoder_opus 		
		 audio_encoder_multiopus 	
		 audio_decoder_multiopus 	


		 #modules/audio_coding 

		 #audio_coding_module_typedefs 	
		 audio_coding 					
		 legacy_encoded_audio_frame 	
		 webrtc_cng 					
		 audio_encoder_cng 				
		 red 							
		 g711 							
		 g711_c 						
		 g722 							
		 g722_c 						
		 ilbc 							
		 ilbc_c 						
		# isac_common 					
		 isac 							
		# isac_bwinfo 					
		 isac_vad 						
		 isac_fix 						
		 isac_fix_common 				
		 isac_fix_c 					
		 pcm16b 						
		 pcm16b_c 						
		 audio_coding_opus_common 		
		 webrtc_opus 					
		 webrtc_multiopus 				
		 webrtc_opus_wrapper 			
		 audio_network_adaptor_config 	
		 audio_network_adaptor 			
		 neteq 							
		 default_neteq_factory 			
		 neteq_tools_minimal 			
		 neteq_test_tools 				
		 neteq_tools 					
		 neteq_input_audio_tools 		
		 #audio_decoder_interface 		
		# audio_encoder_interface 		
		#fake_port_allocator
		#fake_packet_transport
		#fake_dtls_transport 
		#fake_ice_transport
		#fake_stun
		ice
		p2p_peerconnection 

		libmedia

)


target_link_libraries(ice_server_main  ${ALL_LIB} )


target_link_libraries(ice_client_main  ${ALL_LIB} )
target_link_libraries(fake_stun_test  ${ALL_LIB} )



target_link_libraries(libice_main  ${ALL_LIBICE} )





#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /SAFESEH:NO")


set(AllFile ${ice_client_main_source}
			${ice_server_main_source}
			${fake_stun_test_source}
			${libice_main_source}
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
set_property(TARGET fake_stun_test  				PROPERTY FOLDER ${PROJECT_NAME})
set_property(TARGET libice_main  				PROPERTY FOLDER ${PROJECT_NAME})
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

