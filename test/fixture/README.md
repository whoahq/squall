# Test Fixtures

All test fixtures for `squall` are contained in a remotely-hosted tarball.

To populate a local fixture directory, run the following commands from the root
of `squall`:

```bash
cd test/fixture
curl -o squall-fixture.tgz https://whoa.reliquaryhq.com/squall-fixture.tgz
tar -xvzf squall-fixture.tgz
rm squall-fixture.tgz
```

If any tests fail to execute, there's a good chance you need to snag a more
recent tarball and extract it locally.
