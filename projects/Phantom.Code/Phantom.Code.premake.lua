
Phantom_lib("Phantom.Code",  { "Phantom" } -- DEPENDENCIES
	,
	function(Vars) -- include public

	end
	, 
	function(Vars)  -- include private
		defines
		{
			"PHANTOM_LIB_PHANTOM_CODE",
			"_CRT_SECURE_NO_WARNINGS"
		}
					
        Phantom_pch()

		local hauntParams = {}
		hauntParams["IsHaunted"] = true
		Phantom_plugin("Phantom.Code", Vars, hauntParams)

	end
	,
	function(Vars) -- link
	end

)
