@echo off
setlocal EnableDelayedExpansion
pushd "%~dp0"
goto :init_consts

:init_consts
    @REM Colors
    set deco_reset=[0m

    set "text_red=[38;2;255;63;63m"
    set "text_green=[38;2;63;255;63m"
    set "text_blue=[38;2;0;155;255m"
    set "text_purple=[38;2;178;0;255m"
    set "text_yellow=[38;2;255;216;0m"
    set "text_orange=[38;2;255;106;0m"
    
    set "rect_red=[48;2;255;63;63m"
    set "rect_green=[48;2;63;255;63m"
    set "rect_orange=[48;2;255;106;0m"
    set "rect_purple=[48;2;178;0;255m"

    @REM Some texts
    set "banner_error=%rect_red%  [ERROR]  %deco_reset%%text_red%"
    set "banner_success=%rect_green%%text_blue% [SUCCESS] %deco_reset%%text_green%"
    set "banner_warning=%rect_orange% [WARNING] %deco_reset%%text_orange%"

    @REM Emulator paths
    set "emu_paths[0]=%APPDATA%\Citra"
    set "emu_paths[1]=%APPDATA%\Lime3DS"
    set "emu_paths[2]=%APPDATA%\Azahar"
    set "emu_paths[3]=%APPDATA%\AzaharPlus"

    @REM Emulator names
    set "emu_names[0]=Citra"
    set "emu_names[1]=Lime3DS"
    set "emu_names[2]=Azahar"
    set "emu_names[3]=AzaharPlus"

    @REM Others
    set "parenthesis[0]=%text_yellow%("
    set "parenthesis[1]=%text_yellow%)"
    
    goto :check_luma

:check_luma
    @REM Check if the luma folder exists
    if not exist "luma" (
        echo %banner_error% The %text_blue%luma%text_red% folder is missing. Extract the whole archive before running install.bat. If the error still occures, redownload the archive.%deco_reset%
        goto :end
    )
    
    goto :ask

:ask
    echo %text_purple%Where to install the plugin?
    echo Press either A, B or C.
    echo %text_yellow% A %text_purple%- %text_blue%Emulators (Citra, Lime3DS, Azahar, AzaharPlus)
    echo %text_yellow% B %text_purple%- %text_blue%3DS SD card
    echo %text_yellow% C %text_purple%- %text_blue%Both
    echo %text_blue%Your choice:%text_yellow%
    choice /C ABC /N
    echo.

    set "both=0"

    if %errorlevel%==1 (
        goto :install_emu
    ) else if %errorlevel%==2 (
        goto :install_3ds
    ) else if %errorlevel%==3 (
        set "both=1"
        goto :install_emu
    )

:install_emu
    echo %rect_purple%[PLUGIN INSTALLATION ON EMULATORS]%deco_reset%
    set /a found=0

    @REM Iterate through known emulator paths
    for /L %%i in (0,1,3) do (
        set "emu_path=!emu_paths[%%i]!"
        set "emu_name=!emu_names[%%i]!"

        @REM If emu is installed
        if exist "!emu_path!" (
            set /a found+=1
            set "sdmc_path=!emu_path!\sdmc"
            set "sdmc_luma_path=!sdmc_path!\luma"

            @REM Copy the luma folder to the sdmc folder
            xcopy "luma" "!sdmc_luma_path!" /E /I /Y > nul 2>&1
            set "copy_res=!errorlevel!"

            @REM Echo'd variables
            set "emu_name_color=%text_blue%!emu_name!"
            set "emu_path_color=%text_purple%!sdmc_path!"

            if !copy_res!==0 (set res_text=%banner_success% Plugin installed for) else (set res_text=%banner_error% Could not install the plugin for)
            set "full_variables=!emu_name_color! !parenthesis[0]!!emu_path_color!!parenthesis[1]!"

            @REM Full echo
            echo !res_text! !full_variables! %deco_reset%
        )
    )

    if !found!==0 (
        echo %banner_warning% No emulator found.%deco_reset%
    )

    if %both%==1 (
        goto :install_3ds
    ) else (
        goto :end
    )

:install_3ds
    @REM Line break if installing on both
    if %both%==1 (
        echo.
    )

    echo %rect_purple%[PLUGIN INSTALLATION ON 3DS SD CARD]%deco_reset%

    @REM Ask for drive letter
    set /p "letter=%text_blue%Enter the SD card's drive letter: %text_yellow%"

    @REM Ensure the drive letter exists
    set "sd_path=%letter%:\"
    if not exist "%sd_path%" (
        echo %banner_error% SD card path %text_blue%%sd_path% %text_red%does not exist.
        goto :end
    )

    set "driver_disk=%letter%:"

    @REM Error if Windows disk
    if /I "%letter%:"=="%SystemDrive%" (
        if exist "C:\Windows\System32\" (
            echo %banner_error% The %driver_disk% disk is the Windows disk, not your SD card's.
            goto :end
        )
    )

    @REM Ensure drive letter is a 3DS SD card
    if not exist "%letter%:\Nintendo 3DS\" (
        echo %banner_error% The %driver_disk% disk does not seem to be a 3DS SD card.
        goto :end
    )

    set "sd_luma=%driver_disk%\luma"
    xcopy "luma" "%sd_luma%" /E /I /Y > nul 2>&1
    set "copy_res=!errorlevel!"
    
    @REM Echo'd variables
    set "driver_disk_color=%text_purple%%driver_disk%\"
    set "full_variables=!parenthesis[0]!%driver_disk_color%!parenthesis[1]!%deco_reset%"

    if !copy_res!==0 (
        echo %banner_success% Plugin installed on the SD card !full_variables!
    ) else (
        echo %banner_error% Could not install the plugin on the SD card !full_variables!
    )

    goto :end

:end
    echo.
    echo %text_purple%Press any key to exit.%deco_reset%
    pause > nul
    exit