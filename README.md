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

// Resolves when the track finishes (or when stop() is called).
// Rejects if the file cannot be loaded.
await audio.play('/path/to/track.mp3')

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

- **resolves** when the sound finishes naturally or is stopped via `stop()`, and
- **rejects** if the file at `path` cannot be loaded.

### `audio.stop()`

Stop the currently playing sound. No-op if nothing is playing. Resolves the pending `play()` promise and releases the sound's resources.

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
