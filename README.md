# bare-miniaudio

Native [Bare](https://github.com/holepunchto/bare) bindings for [miniaudio](https://miniaud.io/), exposing a small, promise-based API for playing audio files.

The binding wraps miniaudio's high-level engine. Sound-completion events are delivered from miniaudio's audio thread back to the JavaScript thread using a `js_threadsafe_function`, so `play()` can resolve when playback actually finishes.

## Installation

```sh
npm install
```

## Usage

```js
const MiniAudio = require('bare-miniaudio')

const audio = new MiniAudio()

await audio.ready()

// Resolves with true when the track finishes, or false if stop() was called.
// Rejects if the file cannot be loaded.
const ended = await audio.play('/path/to/track.mp3')

if (ended) console.log('played to the end')
else console.log('stopped early')

// Stop playback early from elsewhere:
setTimeout(() => audio.stop(), 1000)
```

## API

### `const audio = new MiniAudio()`

Create an instance. Extends [`ready-resource`](https://github.com/holepunchto/ready-resource).

### `await audio.ready()`

Initialize the underlying miniaudio engine. Must be awaited before calling `play()`.

### `await audio.play(path)`

Load and start playing the file at `path`. Only one sound plays at a time — calling `play()` while another sound is playing stops the current one first.

Returns a promise that:

- **resolves with `true`** when the sound plays through to the end,
- **resolves with `false`** when playback is cut short by `stop()` (including the implicit stop when `play()` is called again), and
- **rejects** if the file at `path` cannot be loaded.

The resolved value lets you tell the two cases apart, e.g. to advance to the next track only on natural completion.

### `audio.stop()`

Stop the currently playing sound. No-op if nothing is playing. Resolves the pending `play()` promise with `false` and releases the sound's resources.

## Building from source

The native addon is built with [`bare-make`](https://github.com/holepunchto/bare-make):

```sh
npm run build   # or:
npx bare-make generate
npx bare-make build
npx bare-make install
```

## License

Apache-2.0
