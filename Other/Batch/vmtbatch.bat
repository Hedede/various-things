for /L %%a in (1,1,8) do (
	if exist blueprints_v%%a.vmt del blueprints_v%%a.vmt
	echo "VertexLitGeneric" >> blueprints_v%%a.vmt
	echo { >> blueprints_v%%a.vmt
	echo 	"$baseTexture" "Models/bp/props/blueprints_v%%a" >> blueprints_v%%a.vmt
	echo } >> blueprints_v%%a.vmt
)