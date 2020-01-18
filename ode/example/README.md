# ODE-example

ODE導入メモ（Windows環境編）

<1>ODEプログラムをコンパイルする．

①このメモと同じフォルダにあるode-0.13.tar.gz(ファイル名はバージョンによって変わる可能性あり)を自分のパソコン内で解凍する．
　以下解凍先のディレクトリを　XX/　で表す．

②コマンドプロンプトを開き，
　ディレクトリを　'XX/ode-0.13.tar/ode-0.13/build' に移動し,
　'premake4 --only-double --with-demos --with-libccd vs2010' を打ち込む.

③ソリューションファイル　'XX/ode-xx/build/vs2010/ode.sln'　をvisual studio 2010で開く．

④visual studio 2010で，ソリューション構築をReleaseDLLにしてビルドする．
　（デバッグ開始の緑の矢印の隣をDebugDLL→ReleaseDLLにしてビルド）

⑤デバッグなしで実行する．成功すれば，青空と地面の画像が出たウィンドウなどが表示される．




<2>任意のプログラムを実行する．

以下では，テストプログラムを実行する過程を通して，実際に自分で組んだシミュレーションプログラムを実行する方法を確認する．


①'XX/ode-0.13.tar/ode-0.13/ode/demo' 内にあるシミュレーションプログラムの中から適当にひとつ選び，シミュレーションプログラムのmain（'demo_***.cpp'の形式のファイル）をコピーする．

②作業フォルダを新しく用意し，①のファイル 'demo_***.cpp' をペーストする．
　以下，このファイルのディレクトリを　'YY/' で表す．

③'YY/'に　'XX/ode-xx/lib/'　
　　　　 　'XX/ode-xx/include/'　
　　　　　 'XX/ode-xx/drawstuff/'　をディレクトリごとコピーする．

④'YY/'に　'YY/lib/ReleaseDLL/drawstuff.dll'　
         　'YY/lib/ReleaseDLL/ode.dll'　      をコピーする．

⑤シミュレーションプログラムを実行するためのファイルを作成する．
　'YY/lib/'にインクルードパスを通して，シミュレーションプログラムのmainをビルドする．


　メモ帳を開き，以下の内容を打ち込み'YY/'に保存する．

----------------------------------------------------------------

del *.obj
del *.exe
call C:\PROGRA~2\MICROS~2.0\VC\vcvarsall

cl demo_***.cpp /I include lib\ReleaseDLL\drawstuff.lib lib\ReleaseDLL\ode.lib

pause

----------------------------------------------------------------

（以下説明）
１行目　実行ファイルを作る途中でできる中間ファイルを削除する．
２行目　Workフォルダの中にある、実行ファイルをまとめて全部削除する．

３行目　visual studio 2010 を呼び出す．（パスはパソコンによって違うので確認すること）

４行目　'demo_***.cpp'部分は開きたいファイル．
　　　　'/I'は，ディレクトリ内でインクルードファイルを検索する指令．
　　　　'include lib\ReleaseDLL\drawstuff.lib lib\ReleaseDLL'はinclude以下　　　　に記述したライブラリーをインクルードする指令．
　　　　
５行目　計算終了後にウィンドウが閉じないための記述




⑥'YY/'に　'XX/ode-xx/ode/demo/demo_***.cpp'　
         　'XX/ode-xx/ode/demo/***_geom.h'　
         　'XX/ode-xx/ode/demo/texturepath.h'　   をコピー．

  'YY/demo_***.cpp'がmainになる．

⑦先ほど作ったメモ帳の名前を'***.txt'から'***.bat'に拡張子を変換し，ダブルクリックすることで'demo_***.cpp'がコンパイルできる．（編集の際は右クリック→編集）

⑧すると，'YY/'に'demo_***.exe'と'demo_***.obj'が作成される．

⑨'demo_***.exe'を開く．成功すればシミュレーションプログラムが開始する．




(注)特定のテクスチャが出ないといったエラーメッセージが吐き出された場合は，'texturepath.h'内のパスの指定がおかしいことが考えられる．'texturepath.h'を開くと
"../../drawstuff/textures"と記述されているところを"./drawstuff/textures"と書き直して⑦からやり直せば解決するはず．



