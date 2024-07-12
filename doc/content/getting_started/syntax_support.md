If you are using [VSCode](https://code.visualstudio.com) we suggest installing the [YAML Extension](https://marketplace.visualstudio.com/items?itemName=redhat.vscode-yaml). This extension enable suggestions and autocomplete for a custom yaml schema. PRISM provides a json file which contains the schema for the PRISM input syntax.

!listing prism/scheme.json

To enable the features of this extension you need configure your settings to allow the extension to know that this cutsom scheme exists. This is an example of how you may do this for files ending in `.p`.

```json
  "yaml.schemas": {
    "scheme.json": "*.p"
  }
```
