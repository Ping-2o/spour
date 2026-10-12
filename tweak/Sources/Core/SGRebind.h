// Hooks a C function Spotify's executable imports, by pointing the image's own import slot for it
// (its __got, or __la_symbol_ptr in an older build) at a replacement. Only calls Spotify's own code
// makes go through the new function; the system's frameworks keep calling the original. This is the
// fishhook technique, for the few places no Objective-C method stands between Spotify and a system
// call (the audio output unit Music Haptics listens to).
//
// Every loaded image is walked rather than the main executable alone: a container (LiveContainer and
// the like) runs the host app as the executable and loads Spotify as a dylib, so Spotify's import
// slots live outside the executable there. The mod's own dylib is skipped so its calls stay its own.
//
// Threading: call from a %ctor, before Spotify has run; the slots are written without a lock.
#import <Foundation/Foundation.h>

// Returns YES when at least one slot named `symbol` (without the leading underscore) was repointed.
// `original` receives what the first slot held, which is what the replacement calls on.
BOOL SGRebindImport(const char *symbol, void *replacement, void **original);
