
isLocal=false
if  [  -n "$1" ] ;then
      if [ "$1" = "true" ]; then
         isLocal=true
      fi
fi
if [ "$isLocal" = "true" ];then
    echo "local build"
else
    echo "remote build"
    sudo sudo npm install cmake-js -g
    mkdir -p ~/.gAssets/fonts/
    git clone https://github.com/alibaba/WmCanvas.git && cp  -R ./WmCanvas/core ./core/  && npm run compile && cp ./third_party/font/wqy-microhei.ttc ~/.gAssets/fonts/wqy-microhei.ttc &&  cp ./third_party/font/.fontcache  ~/.gAssets/fonts/.fontcache
    rm -rf ./WmCanvas/
fi