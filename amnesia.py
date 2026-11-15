from collections import OrderedDict
import json
import sys


def replay(capacity, operations):
    if not isinstance(capacity, int) or isinstance(capacity, bool) or capacity < 0:
        raise ValueError("capacity must be a non-negative integer")

    cache = OrderedDict()
    results = []

    for operation in operations:
        if not isinstance(operation, (list, tuple)) or len(operation) < 2:
            raise ValueError("each operation must contain a kind and a key")

        kind, key, *payload = operation

        if kind == "get":
            if key in cache:
                value = cache[key]
                cache.move_to_end(key)
                results.append(value)
            else:
                results.append(None)

        elif kind == "put":
            if len(payload) != 1:
                raise ValueError("put operation requires exactly one value")

            if capacity == 0:
                continue

            # Updating an existing key must not evict another entry.
            if key in cache:
                cache[key] = payload[0]
                cache.move_to_end(key)
            else:
                if len(cache) >= capacity:
                    cache.popitem(last=False)
                cache[key] = payload[0]

        else:
            raise ValueError(f"unknown operation: {kind!r}")

    return results


def main():
    data = json.load(sys.stdin)
    print(json.dumps(replay(data["capacity"], data["operations"])))


if __name__ == "__main__":
    main()
