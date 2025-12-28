# Changelog

All notable changes to the Asteroids game project.

## [Unreleased] - 2025-12-28

### Added

#### Display & Graphics
- **VSync Support**: Added `--vsync` command-line option for smoother rendering with vertical synchronization
- **Window Mode Configuration**: Expanded window modes from 2 to 4 options:
  - Windowed (resizable)
  - Fullscreen (true fullscreen)
  - Borderless (borderless fullscreen)
  - Maximized (maximized window)
- **F11 Fullscreen Toggle**: Added runtime fullscreen toggling with F11 key (500ms debounce)
- **Display/Mode Validation**: Added validation for `--display` and `--display-mode` options with fallback to safe defaults
- **Renderer Fallback**: Implemented automatic fallback from hardware to software renderer if hardware acceleration fails

#### Performance Optimizations
- **Circle Drawing Optimization**: Pre-calculated sin/cos lookup table, reducing 720 SDL calls per circle to 2 calls using batched rendering
- **SDL Hints Configuration**: Added performance hints for Metal renderer, render batching, and framebuffer acceleration
- **Frame Limiting**: Replaced busy-wait loops with `SDL_Delay(1)` to reduce CPU usage

#### Audio Improvements
- **Volume Control**: Added `--volume=X` option (0-128 range, default: 32)
- **Removed --no-sound**: Replaced with `--volume=0` for consistency
- **Dynamic Audio Paths**: Audio files now loaded using `SDL_GetBasePath()` instead of hardcoded relative paths
- **Memory Leak Fix**: Fixed cleanup bug where GAME_OVER sound chunk wasn't being freed
- **Graceful Degradation**: Added NULL checks for missing audio files with informative warnings

#### Developer Features
- **FPS Display**: Enabled `--show-fps` option to display real-time FPS counter
- **FPS Configuration**: Added `--fps=X` option (1-300 range, default: 60) for custom target frame rates
- **Startup Configuration Logging**: Added detailed logging of all game settings at startup
- **Comprehensive Error Handling**: Improved command-line parsing with validation and helpful error messages

### Changed

#### Error Handling & Logging
- **Standardized Logging**: Created `logger.h` with consistent macros (LOG_SDL_ERROR, LOG_MIX_ERROR, LOG_INFO, LOG_WARN, LOG_INFO_FMT, LOG_WARN_FMT)
- **SDL Error Checking**: Added comprehensive error checking throughout audio.c and graphics.c
- **Command-Line Validation**: Added detection of unknown options with clear error messages
- **Value Range Validation**: Added bounds checking for volume (0-128), FPS (1-300), and window mode (0-3)

#### macOS M1 Fixes
- **Retina Display Fix**: Removed `SDL_RenderSetLogicalSize()` to fix flickering and line segment rendering issues on M1 Macs
- **Color Rendering Fix**: Removed unnecessary `SDL_BLENDMODE_BLEND` that caused washed-out colors
- **Metal Renderer**: Configured Metal as preferred renderer on macOS for best performance

#### Configuration
- **Game Settings Structure**: Updated to include vsync, volume, and other new options
- **Command-Line Flow**: Improved parameter passing from command_line → game_settings → game initialization
- **Default Values**: Set sensible defaults (fullscreen mode, 60 FPS, volume 32, VSync off)

### Documentation

#### README Updates
- Added comprehensive VSync documentation explaining when to enable/disable
- Added window modes table with descriptions and examples
- Added FPS display documentation with use cases
- Added target frame rate configuration guide
- Added audio volume control documentation
- Added complete game controls section (gameplay + system controls)
- Updated examples to reflect current command-line options

### Technical Improvements

#### Code Quality
- Fixed argv[0] being processed in command-line parsing (now correctly starts from argv[1])
- Added return value checking for `parse_argument()` function
- Improved memory management in audio path construction
- Added NULL safety checks in audio chunk operations
- Better separation of concerns in settings initialization

#### SDL2 Best Practices
- Proper SDL hints configuration for optimal performance
- Correct renderer flags for hardware acceleration
- Window positioning with `SDL_WINDOWPOS_CENTERED_DISPLAY`
- Display mode validation before window creation
- Proper resource cleanup on shutdown

## Previous Versions

Version history before this changelog was created is available in git commit history.

---

**Note**: This changelog follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) format.
