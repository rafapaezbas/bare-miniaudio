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
        addon.stop()
        this.isPlaying = false
        this._playResolve(true)
        this._playResolve = null
      })
      if (!loaded) {
        this.isPlaying = false
        reject(path + ' could not be loaded')
      }
    })
  }

  stop() {
    if (!this.isPlaying) return
    addon.stop()
    this.isPlaying = false
    this._playResolve(false)
    this._playResolve = null
  }
}

module.exports = MiniAudio
