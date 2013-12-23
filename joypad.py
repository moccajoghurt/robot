def instructions(state):
  if state[7] == 5 and state[5] == 128:
    print("forward")
  elif state[7] == 5 and state[5] == 127:
    print("back")
  elif state[7] == 5 and state[5] == 0:
    print("stop forward/back")
  elif state[7] == 4 and state[5] == 128:
    print("left")
  elif state[7] == 4 and state[5] == 127:
    print("right")
  elif state[7] == 4 and state[5] == 0:
    print("stop left/right")
  elif state[7] == 1 and state[4] == 1:
    print("NOT STOPP")

pipe = open('/dev/input/js0','r')



while 1:
  state = []
  
  for character in pipe.read(8):
    state.append(ord(character))
    
  #print str(state) + ""
  instructions(state)

