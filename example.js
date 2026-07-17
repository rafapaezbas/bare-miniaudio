const binding = require('./binding')
console.log(binding)
setInterval(() => console.log('beep'), 1000)
console.log(binding.init())
binding.play('/Users/rafa/track.mp3')

setTimeout(() => binding.stop(), 3000)
setTimeout(() => binding.play('/Users/rafa/track-b.mp3'), 4000)
