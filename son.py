import asyncio
import websockets

async def hello():
    while True:
        uri = "ws://localhost:3000"
        async with websockets.connect(uri) as websocket:
            name = input("What's your name? ")

            await websocket.send(name)
            print(f"> {name}")

        #greeting = await websocket.recv()
        #print(f"< {greeting}")

loop = asyncio.get_event_loop()
try:
    asyncio.ensure_future(hello())
    loop.run_forever()
except KeyboardInterrupt:
    pass
finally:
    print("Closing Loop")
    loop.close()
