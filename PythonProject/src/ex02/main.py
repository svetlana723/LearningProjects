import os
import asyncio
from contextlib import suppress
from aioconsole import ainput
from urllib.parse import urlparse, unquote
from requests import get
from prettytable import PrettyTable

async def process(url: str, dir_path: str, table: PrettyTable):
	try:

		parsed = urlparse(url)
		path = unquote(parsed.path)
		filename = os.path.basename(path)		
	   
		full_path = os.path.join(dir_path, filename)
		
		obj = get(url, timeout=10)
		obj.raise_for_status()
		
		with open(full_path, 'wb') as f:
			f.write(obj.content)
		
		table.add_row([url, "Успех"])
		
	except Exception as e:
		table.add_row([url, "Ошибка"])

async def worker(queue: asyncio.Queue, dir_path: str, table: PrettyTable):
	while True:
		url = await queue.get()
		await process(url, dir_path, table)
		queue.task_done()


async def iostream(queue: asyncio.Queue):
	while True:
		url = await ainput("Введите ссылку: ")
		if len(url) == 0:
			break
		await queue.put(url)


async def manager(queue: asyncio.Queue, iostream_task: asyncio.Task, worker_task: asyncio.Task):
	await iostream_task
	await queue.join()
	worker_task.cancel()
	await worker_task

async def main():
	input_message = "Введите путь, по которому следует сохранять загруженные изображения: "
	path = input(input_message)

	while not os.path.exists(path) or not os.access(path, os.W_OK):
		print("Нет доступа или неверно задан путь. Попробуйте ещё раз.")
		path = input(input_message)
	print(path)

	urls = []
	table = PrettyTable(["Ссылка", "Статус"])

	queue = asyncio.Queue()

	worker_task = asyncio.create_task(worker(queue, path, table))
	iostream_task = asyncio.create_task(iostream(queue))
	manager_task = asyncio.create_task(manager(queue, iostream_task, worker_task))

	with suppress(asyncio.exceptions.CancelledError):
		await asyncio.gather(iostream_task, worker_task, manager_task)
	
	print(table)

if __name__ == "__main__":
	asyncio.run(main())