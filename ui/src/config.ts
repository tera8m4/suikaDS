export type ConfigData = {
  ankiPictureField: string;
};

export class Config {
  public static getInstance(): Config {
    if (this.instance === undefined) {
      this.instance = new Config();
    }
    return this.instance;
  }

  private static instance?: Config = undefined;
  private static readonly LOCAL_STORAGE_FIELD: string = "config";
  private data: ConfigData;

  private constructor() {
    this.data = this.loadData();
  }

  public getData(): ConfigData {
    return this.data;
  }

  public update(newData: ConfigData): void {
    this.data = newData;
    localStorage.setItem(Config.LOCAL_STORAGE_FIELD, JSON.stringify(this.data));
  }

  private loadData(): ConfigData {
    const data: ConfigData = JSON.parse(localStorage.getItem(Config.LOCAL_STORAGE_FIELD) || "{}");
    if (data["ankiPictureField"] === undefined) {
      data["ankiPictureField"] = "";
    }

    return data;
  }
}
