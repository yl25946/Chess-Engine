import torch

print(torch.cuda.is_available())

print(torch.cuda.device_count())

print(torch.cuda.current_device())

print(torch.cuda.get_device_name())

print(torch.cuda.memory_allocated())

print(torch.cuda.max_memory_allocated())
