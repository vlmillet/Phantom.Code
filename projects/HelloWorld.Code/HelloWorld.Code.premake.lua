
Phantom_console_app("HelloWorld.Code",  { "Phantom.Code" } -- DEPENDENCIES
	,
	function(Vars)  -- include private
		files { "projects/HelloWorld.Code/**.cpplite" }
	end
	,
	function(Vars) -- link
		filter {}
	end

)
