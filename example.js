const binding = require('./binding')
console.log(binding)
// setInterval(() => console.log('beep'), 1000)
console.log(binding.init())
binding.play('/Users/rafa/track.mp3', () => console.log('finished'))
// binding.stop()

