const ReadyResource = require('ready-resource')
const addon = require.addon()

class MiniAudio extends ReadyResource {
  constructor() {
    super()
    this.isPlaying = false
    this._playResolve = null
  }

  _open() {
    return addon.init()
  }

  play(path) {
    if (this.isPlaying) this.stop()
    this.isPlaying = true
    return new Promise((resolve, reject) => {
      this._playResolve = resolve
      const loaded = addon.play(path, () => {
        this.stop()
      })
      if (!loaded) {
        this.isPlaying = false
        reject(path + ' could not be loaded')
      }
    })
  }

  stop() {
    if (!this.isPlaying) return
    this._playResolve()
    this.isPlaying = false
    this._playResolve = null
    return addon.stop()
  }
}

module.exports = MiniAudio
